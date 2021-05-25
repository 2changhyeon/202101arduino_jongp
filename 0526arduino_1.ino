#include <Wire.h>
#include <sensorbar.h>

#define L_L 19
#define L_R 21    // 0일때 LED켜짐 

volatile byte state = LOW;
volatile boolean line_21;

const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)


SensorBar mySensorBar(SX1509_ADDRESS);


int spd = 200;
int BR_S = 2;   
int BR_D = 22;  
int FR_S = 3;   
int FR_D = 24;  
int BL_S = 4;   
int BL_D = 26;  
int FL_S = 5;   
int FL_D = 28;  

String LTresult = "";
String LTstr[8];
int flagLeft, flagRight;

void setup()
{
  //모터드라이빙용 핀
  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(28,OUTPUT);
  analogWrite(BR_S, 0);   //PWM Speed Control
  analogWrite(FR_S, 0);   //PWM Speed Control
  analogWrite(BL_S, 0);   //PWM Speed Control
  analogWrite(FL_S, 0);   //PWM Speed Control
  
  //인터럽트용 핀
  pinMode(L_R, INPUT);
  pinMode(L_L, INPUT);
  attachInterrupt(digitalPinToInterrupt(19),toggle19, RISING); //둘다 1일때 인터럽트 발생 
  

  //라인트레이싱 시작 SPARKFUN
  Serial.begin(9600);
  Serial.println("Program started.");
  Serial.println();
  
  mySensorBar.setBarStrobe();
  mySensorBar.clearInvertBits();
  mySensorBar.clearInvertBits();


  uint8_t returnStatus = mySensorBar.begin();
  
  if(returnStatus)
  {
    Serial.println("sx1509 IC communication OK");
  }
  else
  {
    Serial.println("sx1509 IC communication FAILED!");
    while(1);
  }
  Serial.println();
  
}

void loop()
{ 
  
  line_21 = digitalRead(L_L);
  boolean line_19 = digitalRead(L_R);

  Serial.println(line_21);
  Serial.println(line_19);
  
  getLT();
  driving();
  delay(1000);

}

void toggle19()
{
  if(line_21 == HIGH)
  {
  state = !state;
  Serial.println("인터럽트 발생");
  }
  else
  {
  Serial.println("인터럽트 x");    
  }

}

void getLT()
{
  LTresult = "";
  uint8_t rawValue = mySensorBar.getRaw();
  int i;
  flagRight = 0;
  flagLeft = 0;
  
  for( int i = 7; i >= 0; i-- )
  {
    LTstr[i] = String(((rawValue >> i) & 0x01));
  }
  
  if(LTstr[0]=="1")
  {
    flagLeft = 1;
  }
  
    if(LTstr[7]=="1")
  {
    flagRight = 1;
  }
  
  for( int i = 0; i<8; i++ )
  {
    LTresult.concat(LTstr[i]);
  }
  Serial.println(LTresult);

}


void driving()
{
   
  //전진
  if(LTresult == "00011000")
  {
    forward();
  }

  else if(LTresult == "00010000")
  {
    forward();
  }
  
  else if(LTresult == "00001000")
  {
    forward();
  }


  //좌회전
  else if(LTresult == "01100000")
  {
    forward_left();
  }

  else if(LTresult == "01000000")
  {
    forward_left();
  }
  
  else if(LTresult == "00100000")
  {
    forward_left();
  }

  //우회전
  else if(LTresult == "00000110")
  {
    forward_right();
  }

  else if(LTresult == "00000100")
  {
    forward_right();
  }
  
  else if(LTresult == "00000010")
  {
    forward_right();
  }

  else
  {
    turn_right();
  }
  
}
void forward() 
{
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
  Serial.println("전진");  
}

void forward_right() 
{
  digitalWrite(BR_D, HIGH);
  digitalWrite(FR_D, HIGH);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, 30);   
  analogWrite(FR_S, 30);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
  Serial.println("우회전");  
}

void forward_left() 
{
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, HIGH);
  digitalWrite(FL_D, HIGH);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, 30);   
  analogWrite(FL_S, 30);
  Serial.println("좌회선");     
}

void stopp() 
{
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, 0);   
  analogWrite(FR_S, 0);   
  analogWrite(BL_S, 0);   
  analogWrite(FL_S, 0); 
  Serial.println("스톱");    
}

void turn_right() 
{
  digitalWrite(BR_D, HIGH);
  digitalWrite(FR_D, HIGH);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);
  Serial.println("돌아");     
}

void turn_left() 
{
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, HIGH);
  digitalWrite(FL_D, HIGH);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
}
