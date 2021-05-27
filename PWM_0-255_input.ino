
#define  BR_S 2  
#define  BR_D 22  
#define  FR_S 3   
#define  FR_D 26 
#define  BL_S 4  
#define  BL_D 30 
#define  FL_S 5 
#define  FL_D 34  

int spd = 0;
 
void setup() 
{ 
  pinMode(22,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(30,OUTPUT);
  pinMode(34,OUTPUT);
  analogWrite(BR_S, 0);   //PWM Speed Control
  analogWrite(FR_S, 0);   //PWM Speed Control
  analogWrite(BL_S, 0);   //PWM Speed Control
  analogWrite(FL_S, 0);   //PWM Speed Control
  
  Serial.begin(9600);
  
  while (! Serial);
  
  Serial.println("Speed 0 to 255");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
    int spd = Serial.parseInt();
    if (spd >= 0 && spd <= 255)
    {
    digitalWrite(BR_D, HIGH);
    digitalWrite(FR_D, HIGH);
    digitalWrite(BL_D, HIGH);
    digitalWrite(FL_D, HIGH);
    
    analogWrite(BR_S, spd);   
    analogWrite(FR_S, spd);   
    analogWrite(BL_S, spd);   
    analogWrite(FL_S, spd);  

    delay(2000);
    }
  }
} 
