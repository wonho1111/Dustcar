#include<SoftwareSerial.h>
#include<Servo.h>
SoftwareSerial s(2,3);
Servo servo;

#define trig1 10 // 장애물
#define echo1 11

#define trig2 12 // 사람 손
#define echo2 13

int Ls = 7;
int Rs = 8;
int motor = 5;
int angle = 0;

void setup() {
  servo.attach(motor);
  pinMode(Ls, INPUT);
  pinMode(Rs, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  
  Serial.begin(9600);
  s.begin(9600);
}

void loop() {

  long duration1, distance1;
  long duration2, distance2;

  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);// trigPin에서 초음파 발생(echoPin도 HIGH)        
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  
  duration1 = pulseIn(echo1,HIGH);
  distance1 = ((float)(340*duration1)/10000)/2;

  delay(500);

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);

  duration2 = pulseIn(echo2,HIGH);
  distance2 = ((float)(340*duration2)/10000)/2;


  if(distance2 < 10) {
    s.write(51);
    Serial.println("4:정지");
    Serial.print("사람손 거리: ");
    Serial.print(distance2);
    Serial.println("cm");

    angle=90;
    Serial.println("open");
    delay(500);
    servo.write(angle);
    
  }
  else {
  
    angle = 0;
    servo.write(angle);
    Serial.println("close");
  
    if(distance1 > 10) {
      // line tracer
      if(digitalRead(Ls)==LOW && digitalRead(Rs)==LOW)//직진
        {
          s.write(48);//'0'전송
          Serial.println("0:직진");
        }
      else if(digitalRead(Ls)==LOW && digitalRead(Rs)==HIGH)//좌회전
        {
          s.write(49);//'1'전송
          Serial.println("1:좌회전");
        }
      else if(digitalRead(Ls)==HIGH && digitalRead(Rs)==LOW)//우회전
        {
          s.write(50);//'2'전송
          Serial.println("2:우회전");
        }
      else if(digitalRead(Ls)==HIGH && digitalRead(Rs)==HIGH)
        {
          s.write(51);
          Serial.println("4:정지");
          Serial.print("거리: ");
          Serial.print(distance1);
          Serial.println("cm");
        }  
      }
      else {
          s.write(51);
          Serial.println("4:정지");
          Serial.print("장애물 거리: ");
          Serial.print(distance1);
          Serial.println("cm");
      }
  }
  
  
  delay(500);
}
