#include <Wire.h>
#include <sensorbar.h>
#define L_L 19
#define L_R 21    // 0일때 LED켜짐 

//const byte L_L = 21;
//const byte L_R = 19;
volatile boolean line_21;
volatile byte state = LOW;

void setup()
{
  pinMode(L_R, INPUT);
  pinMode(L_L, INPUT);

  attachInterrupt(digitalPinToInterrupt(19),toggle19,CHANGE); //둘다 1일때 인터럽트 발생 
  
  //digitalPinToInterrupt(19) && 
  Serial.begin(9600);
  Serial.println("Program started.");
  Serial.println();
  
}

void loop()
{
  line_21 = digitalRead(L_L);
  boolean line_19 = digitalRead(L_R);

  Serial.println(line_21);
  Serial.println(line_19);
  
  delay(500);
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
