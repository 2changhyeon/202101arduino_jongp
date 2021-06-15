#include <Ultrasonic.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(9, 8); //Connect HC-06. Use your (TX, RX) settings
Ultrasonic ultrasonic(10,11); // (Trig PIN,Echo PIN)

//모터A 컨트롤
int enA = 6;
int IN1 = 7;
int IN2 = 5;
  
//모터B 컨트롤
int enB = 3;
int IN3 = 4;
int IN4 = 2;

String receiveStr=""; //받는 문자열
String olfReceiveStr=""; //받는 문자열
#define MOTER_SPEED 255 // MAX 255

void setup()  
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Hello!");

  btSerial.begin(9600);  // set the data rate for the BT port
}

void loop()
{
  analogWrite(enA, MOTER_SPEED);
  analogWrite(enB, MOTER_SPEED);
  
  int value = ultrasonic.Ranging(CM);
  Serial.print(value); // CM or INC
  Serial.print(" cm / " );

  if (value <= 15) {
    //stopAllMotor();
    loop_bluetooth(false);
  } else {
    loop_bluetooth(true);
    //loop_line_tracer();
  }

  Serial.print("\n");
}

void loop_bluetooth(boolean is_forward) {
  // put your main code here, to run repeatedly:
  receiveStr="";
  while(btSerial.available())  //mySerial에 전송된 값이 있으면
  {
    char myChar = (char)btSerial.read();  //mySerial int 값을 char 형식으로 변환
    receiveStr+=myChar;   //수신되는 문자를 receiveStr에 모두 붙임 (1바이트씩 전송되는 것을 연결)
    delay(5);           //수신 문자열 끊김 방지
  }
 
  if (receiveStr.length() > 0) {
    Serial.print(receiveStr);
    if (receiveStr == "F") {
      if (is_forward) {
        bothMotorStart(); // 전진
      } else {
        stopAllMotor();
      }      
    } else if (receiveStr == "S") {
      stopAllMotor();
    } else if (receiveStr == "R") {
      turnRight(); // 오른쪽으로 턴
    } else if (receiveStr == "L") {
      turnLeft(); // 왼쪽으로 턴
    } else if (receiveStr == "B") {
      bothMotorBack(); // 후진
    }
    olfReceiveStr = receiveStr;
  } else if (!is_forward && olfReceiveStr == "F") {
    stopAllMotor();
  }
}

//모터A,B 정회전
void bothMotorStart()
{
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
}

//모터A,B 역회전
void bothMotorBack()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
}
  
//모터A,B Stop
void stopAllMotor()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
}
  
//모터A 역회전, 모터B 정회전
void turnLeft()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
}
  
//모터A 정회전, 모터B 역회전
void turnRight()
{
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
}
  
//모터A 정회전, 모터B Stop
void motorA_Rotation()
{
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
}
  
//모터A Stop, 모터B 정회전
void motorB_Rotation()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
}
  
//모터A 역회전, 모터B Stop
void motorA_Reverse()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
}
  
//모터A Stop, 모터B 역회전
void motorB_Reverse()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
}



출처: https://bluexmas.tistory.com/769 [파란크리스마스]
