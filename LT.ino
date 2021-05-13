#include "Wire.h"
#include "sensorbar.h"

// Uncomment one of the four lines to match your SX1509's address
//  pin selects. SX1509 breakout defaults to [0:0] (0x3E).
const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
//const byte SX1509_ADDRESS = 0x3F;  // SX1509 I2C address (01)
//const byte SX1509_ADDRESS = 0x70;  // SX1509 I2C address (10)
//const byte SX1509_ADDRESS = 0x71;  // SX1509 I2C address (11)

SensorBar mySensorBar(SX1509_ADDRESS);

int spd = 60;
int BR_S = 2;   
int BR_D = 22;  
int FR_S = 3;   
int FR_D = 24;  
int BL_S = 4;   
int BL_D = 26;  
int FL_S = 5;   
int FL_D = 28;  
uint8_t raw;

void setup()
{
  Serial.begin(9600);  // start serial for output
  Serial.println("Program started.");
  Serial.println();
  
  //mySensorBar.setBarStrobe();
  mySensorBar.clearBarStrobe();
  mySensorBar.clearInvertBits();
  //mySensorBar.setInvertBits();

  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(28,OUTPUT);
  analogWrite(BR_S, 0);   //PWM Speed Control
  analogWrite(FR_S, 0);   //PWM Speed Control
  analogWrite(BL_S, 0);   //PWM Speed Control
  analogWrite(FL_S, 0);   //PWM Speed Control
  //Don't forget to call .begin() to get the bar ready.  This configures HW.
  uint8_t returnStatus = mySensorBar.begin();
  if(returnStatus)
  {
    Serial.println("sx1509 IC communication OK");
  }
  else
  {
    Serial.println("sx1509 IC communication FAILED!");
  }
  Serial.println();
  
}

void loop()
{
  int num = 0;
  uint8_t den = mySensorBar.getDensity();
  uint8_t raw = mySensorBar.getRaw();
  Serial.println(raw);
  Serial.println(den);
  num = (int)raw;
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, 0);   
  analogWrite(FR_S, 0);   
  analogWrite(BL_S, 0);   
  analogWrite(FL_S, 0); 
  /*if( den > 5 || den == 0 )
  {
    st();
  }
  else
  {
    if (num == 24||num == 16||num == 8) 
    {
     fw();
    }
    else if (num >= 32)
    {
     fl();  
    }
    else if (num <= 7)
    {
     fr();  
    }
  }*/
  
}

void fw() {
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
}

void fr() {
  digitalWrite(BR_D, HIGH);
  digitalWrite(FR_D, HIGH);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, 30);   
  analogWrite(FR_S, 30);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
}

void fl() {
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, HIGH);
  digitalWrite(FL_D, HIGH);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, 30);   
  analogWrite(FL_S, 30);   
}

void st() {
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, 0);   
  analogWrite(FR_S, 0);   
  analogWrite(BL_S, 0);   
  analogWrite(FL_S, 0);   
}

void rt() {
  digitalWrite(BR_D, HIGH);
  digitalWrite(FR_D, HIGH);
  digitalWrite(BL_D, LOW);
  digitalWrite(FL_D, LOW);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
}

void lt() {
  digitalWrite(BR_D, LOW);
  digitalWrite(FR_D, LOW);
  digitalWrite(BL_D, HIGH);
  digitalWrite(FL_D, HIGH);
  analogWrite(BR_S, spd);   
  analogWrite(FR_S, spd);   
  analogWrite(BL_S, spd);   
  analogWrite(FL_S, spd);   
}
