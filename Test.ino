///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// RC CAR 조립 검사를 하기 위한 테스트 프로그램 입니다.
// 건전지를 연결하지 않고 USB 케이블을 연결한 상태에서
// 블루투스모듈(HC-06)을 제외한 검사가 가능합니다.
//

#include <Servo.h>
Servo myservo; 

#define DIRA               4       // Motor A Direction  (A 회전방향)
#define DIRB               7       // Motor B Direction  (B 회전방향)
#define PWMA               5       // Motor A PWM        (A 속도)
#define PWMB               6       // Motor B PWM        (B 속도)
#define FRONT_LEFT_LED    19       // Analog 5
#define FRONT_RIGHT_LED    2       // D2
#define REAR_LEFT_LED      3       // D3
#define REAR_RIGHT_LED    18       // Analog 4
#define BUZER             17       // Analog 3
#define HCSR04_TRIG       11       // D11
#define HCSR04_ECHO       12       // D12
#define CDS               A0       // Analog 0

int sensorValue = 0;               //  variable
int number = 0;
int pos = 0;                       // variable to store the servo position
long int distance = 0;
long int duration = 0;

///////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(FRONT_LEFT_LED, OUTPUT);
  pinMode(FRONT_RIGHT_LED, OUTPUT);
  pinMode(REAR_LEFT_LED, OUTPUT);
  pinMode(REAR_RIGHT_LED, OUTPUT);
  pinMode(BUZER, OUTPUT);
  pinMode(HCSR04_TRIG, OUTPUT);
  pinMode(HCSR04_ECHO, INPUT);
   
  Serial.begin(9600);  // initialize serial communications at 9600 bps:
  myservo.attach(9);    // attaches the servo on pin 9 to the servo object
}

///////////////////////////////////////////////////////////////////////////

void loop() {

  Serial.print("No. = ");
  Serial.print (number++);
  
// <1> CDS 테스트
// CDS는 빛의 세기를 감지하는 센서입니다. 
// 빛의 세기가 크면 숫자가 크게 표시됩니다.
  sensorValue = analogRead(CDS);        
  Serial.print("  CDS = ");
  Serial.print(sensorValue);
  
// <2> HCSR_04 초음파 센서 테스트
// 초음파를 이용하여 거리를 감지합니다.
// 측정 거리가 cM 단위로 표시됩니다.
 digitalWrite(HCSR04_TRIG, LOW); delayMicroseconds(2); 
 digitalWrite(HCSR04_TRIG, HIGH); delayMicroseconds(10); 
 digitalWrite(HCSR04_TRIG, LOW); duration = pulseIn(HCSR04_ECHO, HIGH);
//
 distance = duration/58.2;
 Serial.print("  Distance = ");
 Serial.print(distance);
 Serial.println(" CM");
//
 delay(300);
 
// <3> SG90 서보 테스트
// 서보를 동작합니다. 회전각은 20도에서 160 범위 입니다.
    for (pos = 90; pos <= 160; pos += 1) { 
    myservo.write(pos); delay(15);                      
  }
  for (pos = 160; pos >= 20; pos -= 1) { 
    myservo.write(pos); delay(15);                      
  }
  for (pos = 20; pos <= 90; pos += 1) { 
    myservo.write(pos); delay(15);                     
  }

// <4> LED 테스트
//전방 힌색LED 와 후방 적색 LED 의점등을 확인합니다.
for (pos = 0; pos < 10; pos++) {     
  digitalWrite(FRONT_LEFT_LED, HIGH); delay(250);
  digitalWrite(FRONT_LEFT_LED, LOW); 
  digitalWrite(FRONT_RIGHT_LED, HIGH); delay(250);
  digitalWrite(FRONT_RIGHT_LED, LOW); 
  digitalWrite(REAR_RIGHT_LED, HIGH); delay(250);
  digitalWrite(REAR_RIGHT_LED, LOW); 
  digitalWrite(REAR_LEFT_LED, HIGH); delay(250);
  digitalWrite(REAR_LEFT_LED, LOW); 
}

// <5> BUZER 테스트
//버져를 테스트 합니다.
  for (pos = 0; pos < 1000; pos++) {          
    digitalWrite(BUZER, HIGH); delay(1);
    digitalWrite(BUZER, LOW);  delay(1);
  }
  
// <6> 왼쪽 DC 모터 테스트
// 모터쉴드의 A+, A- LED가 점등됩니다.
// (USB 의 전류가 작으므로 모터가 회전하지는 않습니다.)
     digitalWrite(DIRA, HIGH);  analogWrite(PWMA, 200); 
     digitalWrite(DIRB, LOW);   analogWrite(PWMB, 0); 
     delay(1000);
     analogWrite(PWMA, 0);

// <7> 오른쪽 DC 모터 테스트
// 모터쉴드의 B+, B- LED가 점등됩니다.
// (USB 의 전류가 작으므로 모터가 회전하지는 않습니다.)
     digitalWrite(DIRA, LOW);   analogWrite(PWMA, 0); 
     digitalWrite(DIRB, HIGH);  analogWrite(PWMB, 200); 
     delay(1000);
     analogWrite(PWMB, 0);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////



