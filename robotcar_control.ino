#include<SoftwareSerial.h>
#include<Servo.h>
SoftwareSerial s(2,3); // rx, tx
Servo servo;

#define trig1 10 // 장애물
#define echo1 11

#define trig2 12 // 사람 손
#define echo2 13

#define trig3 5 // 내부
#define echo3 6

const int Ls = 7;
const int Rs = 8;
const int Cs = 9;
const int motor = 4;
int angle = 0;

void setup() {
  servo.attach(motor);
  pinMode(Ls, INPUT);
  pinMode(Rs, INPUT);
  pinMode(Cs, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);

  Serial.begin(9600);
  s.begin(9600);
}

long sensorset(int trig, int echo) {

  long duration, distance;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);// trigPin에서 초음파 발생(echoPin도 HIGH)        
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  duration = pulseIn(echo,HIGH);
  distance = ((float)(340*duration)/10000)/2;
  delay(500);
  
  return distance;    
}

void loop() {

  long distance1 = sensorset(trig1, echo1); // 전방 장애물 감지
  long distance2 = sensorset(trig2, echo2); // 사람 손 감지
  long distance3 = sensorset(trig3, echo3); // 쓰레기통 용량 감지

  if(distance3 < 2 && digitalRead(Cs)==HIGH) { 
    // 쓰레기통 용량 감지 and 좌측 적외선센서 검정색 감지
    s.write(51);
    Serial.println("FULL");
  }
  else {
    // 사람 손 감지 됐을 때 정지, 서보모터 동작
    if(distance2 < 10) { 
      s.write(51);
      Serial.println("3:정지");
      Serial.print("사람손 거리: ");
      Serial.print(distance2);
      Serial.println("cm");

      for(int i = 0; i < 120; i++)  // 현재 각도에서 120도 더해주기
      {
        angle = angle + 1;
        if(angle >= 120)
            angle = 120;  
        servo.write(angle);
        delay(10);
      }
      delay(5000);
      Serial.println("open");
     
    }
    else { 
      // 사람 손 감지 안됐을 때 주행
      if(angle > 0) {
        for(int i = 0 ; i < 120 ; i++) { // 현재 각도에서 120도 빼주기
          angle = angle - 1;
          if(angle <= 0)
            angle = 0;
          servo.write(angle);
          delay(10);
        }    
      }
      angle = 0;
      servo.write(angle);
      Serial.println("close");
   
      if(distance1 > 10) { 
        // 장애물 감지 안됐을 때 주행
        if(digitalRead(Ls)==HIGH && digitalRead(Rs)==HIGH) { //직진      
          s.write(48);//'0'전송
          Serial.println("0:직진");
        }
        else if(digitalRead(Ls)==HIGH && digitalRead(Rs)==LOW) { //좌회전
  
          s.write(49);//'1'전송
          Serial.println("1:좌회전");
        }
        else if(digitalRead(Ls)==LOW && digitalRead(Rs)==HIGH) { //우회전      
          s.write(50);//'2'전송
          Serial.println("2:우회전");
        }
        else if(digitalRead(Ls)==LOW && digitalRead(Rs)==LOW) { // 적외선 센서 2개다 감지 안됐을 때 정지
          s.write(51);
          Serial.println("3:정지");
          Serial.print("거리: ");
          Serial.print(distance1);
          Serial.println("cm");
        }  
      }
      else { 
        // 장애물 감지 됐을때 정지
        s.write(51);
        Serial.println("3:정지");
        Serial.print("장애물 거리: ");
        Serial.print(distance1);
        Serial.println("cm");
      }
    }
  }
   
  delay(500);
}
