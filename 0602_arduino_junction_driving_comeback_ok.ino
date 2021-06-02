#include <Wire.h>
#include <sensorbar.h>

#define FL_D 22
#define FL_S 2
#define BL_D 26
#define BL_S 3
#define FR_D 30
#define FR_S 6
#define BR_D 34
#define BR_S 5

//ground 제대로 물려주기.
int speed = 100;
int j = 1;

//JUNCTION
long randNumber;
int QR = 33; // qr인풋에서 11을 더해줘야함.
int QRsum = (QR % 10) + (QR / 10);
int QR_x = QR / 10;
int QR_y = QR % 10;
  
int RC_x = 0;
int RC_y = 0;
  
int randn = 0;
int flag = 0;
int goal = 0;

//LT센서
String LTresult = "";
String LTstr[8];

const uint8_t SX1509_ADDRESS = 0x3E; 
SensorBar mySensorBar(SX1509_ADDRESS);

void setup() 
{
  //랜덤함수를 사용하기 위해서 시드 설정
  randomSeed(analogRead(0));
  
  pinMode(FL_D, OUTPUT);         
  pinMode(BL_D, OUTPUT);         
  pinMode(FR_D, OUTPUT);
  pinMode(BR_D, OUTPUT);
  
  analogWrite(FL_S, 0); 
  analogWrite(BL_S, 0); 
  analogWrite(FR_S, 0);  
  analogWrite(BR_S, 0);
  
  Serial.begin(9600);
  Serial.println("Program started.");
  Serial.println();
  
  mySensorBar.clearBarStrobe();
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

  getLT();
  driving();
  delay(50);
}
void getLT()
{
  LTresult = "";
  int i;
  uint8_t rawValue = mySensorBar.getRaw();
    
  for( int i = 7; i >= 0; i-- )
  {
    LTstr[i] = String(((rawValue >> i) & 0x01));
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
  {forward();}

  else if(LTresult == "00010000")
  {forward();}
  
  else if(LTresult == "00001000")
  {forward();}


  //좌회전
  else if(LTresult == "01100000")
  {forward_left();}

  else if(LTresult == "01000000")
  {forward_left();}
  
  else if(LTresult == "00100000")
  {forward_left();}
  
  else if(LTresult == "00110000")
  {soft_forward_left();}

  //우회전
  else if(LTresult == "00000110")
  {forward_right();}

  else if(LTresult == "00000100")
  {forward_right();}
  
  else if(LTresult == "00000010")
  {forward_right();}

  else if(LTresult == "00001100")
  {soft_forward_right();}
  
  else if(LTresult == "11111111")
  {
    injunction();
  }
  else
  {stopp();}  
}

void forward()
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, speed);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, speed);           

  digitalWrite(FR_D, HIGH);            
  analogWrite(FR_S, speed);             

  digitalWrite(BR_D, HIGH);          
  analogWrite(BR_S, speed); 
  Serial.println("전진");

}

void forward_right()
{
  digitalWrite(FL_D, LOW);           
  analogWrite(FL_S, speed/2);            

  digitalWrite(BL_D, LOW);            
  analogWrite(BL_S, speed/2);           

  digitalWrite(FR_D, HIGH);            
  analogWrite(FR_S, speed);             

  digitalWrite(BR_D, HIGH);          
  analogWrite(BR_S, speed); 
}

void soft_forward_right()
{
  digitalWrite(FL_D, LOW);           
  analogWrite(FL_S, speed*0.8);            

  digitalWrite(BL_D, LOW);            
  analogWrite(BL_S, speed*0.8);           

  digitalWrite(FR_D, HIGH);            
  analogWrite(FR_S, speed);             

  digitalWrite(BR_D, HIGH);          
  analogWrite(BR_S, speed); 
}

void forward_left()
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, speed);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, speed);           

  digitalWrite(FR_D, LOW);            
  analogWrite(FR_S, speed/2);             

  digitalWrite(BR_D, LOW);          
  analogWrite(BR_S, speed/2); 
}

void soft_forward_left()
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, speed);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, speed);           

  digitalWrite(FR_D, LOW);            
  analogWrite(FR_S, speed*0.8);             

  digitalWrite(BR_D, LOW);          
  analogWrite(BR_S, speed*0.8); 
}
void stopp()
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, 0);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, 0);           

  digitalWrite(FR_D, LOW);            
  analogWrite(FR_S, 0);             

  digitalWrite(BR_D, LOW);          
  analogWrite(BR_S, 0); 
}

void turn_left() 
{
  digitalWrite(FL_D, LOW);           
  analogWrite(FL_S, 50);            

  digitalWrite(BL_D, LOW);            
  analogWrite(BL_S, 50);           

  digitalWrite(FR_D, HIGH);            
  analogWrite(FR_S, 50);             

  digitalWrite(BR_D, HIGH);          
  analogWrite(BR_S, 50);
  Serial.println("왼쪽회전");

}

void turn_right() 
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, 50);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, 50);           

  digitalWrite(FR_D, LOW);            
  analogWrite(FR_S, 50);             

  digitalWrite(BR_D, LOW);          
  analogWrite(BR_S, 50);
  Serial.println("오른쪽회전");
}

void test()
{
  digitalWrite(FL_D, HIGH);           
  analogWrite(FL_S, 50);            

  digitalWrite(BL_D, HIGH);            
  analogWrite(BL_S, 50);           

  digitalWrite(FR_D, HIGH);            
  analogWrite(FR_S, 50);             

  digitalWrite(BR_D, HIGH);          
  analogWrite(BR_S, 50); 
}

void injunction()
{
  test(); // 속도50으로 1초 직진
  delay(1000);

  if(goal == 0)
  {
    JunctionSelect();
  }
  else if (goal == 1)
  {
    Comeback();
  }
  else
  {
    stopp();
    delay(10000);
  }
}


void Comeback()
{
    randNumber = random(10,20);
    randn = randNumber % 2;


    if ((randn == 0) && (RC_x == 0))
    {
      randn = 1; 
    }
    else if ((randn == 1) && (RC_y == 0))
    {
      randn = 0; 
    }

    
    if((randn == 0) && (RC_x > 0)) //x축이동
    {
      if(flag == 1) //왼쪽을 보고있을때
      {
        forward();
        delay(1000);
        RC_x -= 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
      else
      {
        turn_left();
        delay(3500);
        RC_x -= 1;
        flag = 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    } 
     
    else if((randn == 1)&&(RC_y > 0)) //y축이동
    {
      if(flag == 1) //왼쪽보고있을떄
      {
        turn_right();
        delay(3500);
        RC_y -= 1;
        flag = 0;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
  
      else
      {
        forward();
        delay(1000);
        RC_y -= 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    } 
  
  if((0 == RC_y)&&(0 == RC_x))  
  {
    Serial.println("복귀완료");
  }
}
void JunctionSelect()
{
    randNumber = random(10,20);
    randn = randNumber % 2;

    if ((randn == 0) && (RC_x == QR_x))
    {
      randn = 1; 
    }
    else if ((randn == 1) && (RC_y == QR_y))
    {
      randn = 0; 
    }


  
    if((randn == 0) && (QR_x > RC_x)) //x축이동
    {
      if(flag == 1) //오른쪽을 보고있을때
      {
        forward();
        delay(1000);
        RC_x += 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
      else
      {
        turn_right();
        delay(3500);
        RC_x += 1;
        flag = 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    }
     
    else if((randn == 1)&&(QR_y > RC_y)) //y축이동
    {
      if(flag == 1) //오른쪽보고있을떄
      {
        turn_left();
        delay(3500);
        RC_y += 1;
        flag = 0;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
  
      else
      {
        forward();
        delay(1000);
        RC_y += 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    }
  
  if((QR_y == RC_y)&&(QR_x == RC_x))  
  {
    Serial.println("이동완료");
    goal = 1;
  }
}
