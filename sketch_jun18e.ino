#include <Wire.h>
#include <sensorbar.h>
#include <usbhid.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

#define FL_D 22 // 움직이는 모터
#define FL_S 2 // 움직이는 모터
#define BL_D 26 // 움직이는 모터
#define BL_S 3 // 움직이는 모터
#define FR_D 30 // 움직이는 모터
#define FR_S 6 // 움직이는 모터
#define BR_D 34 // 움직이는 모터
#define BR_S 5 // 움직이는 모터

#define G_1 42 // 지게발
#define G_3 44 // 지게발
#define G_2 12 // pwm지게발

#define LT_L 38 // 목적지용
#define LT_R 40 // 목적지용


//ground 제대로 물려주기.
int speed = 100;  // 속도
int j = 1; //QR
int gi = 0; //지게발
int line_R = 0; //목적지용
int line_L = 0; // 목적지용

//JUNCTION
long randNumber;
int QR_x =0;
int QR_y =0;

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

//qr
class MyParser : public HIDReportParser {
  public:
    MyParser();
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
    void putAscii (uint8_t a);
  protected:
    uint8_t KeyToAscii(bool upper, uint8_t mod, uint8_t key);
    virtual void OnKeyScanned(bool upper, uint8_t mod, uint8_t key);
    virtual void OnScanFinished();
    
};

//qr
uint8_t code[2] = {0,0};
int cnt = 0;
MyParser::MyParser() {}


//qr
void MyParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  // If error or empty, return
  if (buf[2] == 1 || buf[2] == 0) return;

  for (uint8_t i = 7; i >= 2; i--) {
    // If empty, skip
    if (buf[i] == 0) continue;

    // If enter signal emitted, scan finished
    if (buf[i] == UHS_HID_BOOT_KEY_ENTER) {
      OnScanFinished();
    }

    // If not, continue normally
    else {
      // If bit position not in 2, it's uppercase words
      OnKeyScanned(i > 2, buf, buf[i]);
    }

    return;
  }
}


//qr
uint8_t MyParser::KeyToAscii(bool upper, uint8_t mod, uint8_t key) 
{  
  // Letters
  if (VALUE_WITHIN(key, 0x04, 0x1d)) {
    if (upper) return (key - 4 + 'A');
    else return (key - 4 + 'a');
  }

  // Numbers
  else if (VALUE_WITHIN(key, 0x1e, 0x27)) 
  {
    return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
  }

  return 0;
}

//qr
void MyParser::OnKeyScanned(bool upper, uint8_t mod, uint8_t key) 
{
  uint8_t ascii = KeyToAscii(upper, mod, key);

  if(cnt==0)
  {
    QR_x = ascii;
  }
  else
  {
    QR_y = ascii;
  }
  cnt++;
}


//qr
void MyParser::OnScanFinished() 
{
    Serial.println(" - Finished");
    for (int i = 0; i < 4; i++) {
        Serial.print((char)code[i]);
    }
    cnt = 0;
}

//qr
USB          Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);
MyParser     Parser;



void setup() 
{
  //랜덤함수를 사용하기 위해서 시드 설정
  randomSeed(analogRead(0));
  
  pinMode(FL_D, OUTPUT);         
  pinMode(BL_D, OUTPUT);         
  pinMode(FR_D, OUTPUT);
  pinMode(BR_D, OUTPUT);

  pinMode(G_1, OUTPUT);
  pinMode(G_3, OUTPUT);
  
  analogWrite(G_2, 0);

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

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }
  delay(200);
  Hid.SetReportParser(0, &Parser);
}

void loop() 
{
    digitalWrite(G_1, LOW); 
    digitalWrite(G_3, HIGH); 
              
    analogWrite(G_2, 120);            
    delay(1000); //확인필요
    
    digitalWrite(G_1, HIGH); 
    digitalWrite(G_3, LOW); 
              
    analogWrite(G_2, 0);            
    delay(300000); //확인필요    
}


void getLT() //스파크썬 라인트레이서 사용
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
}

void LT() // 출도착지 확인용 라인트레이서
{
  line_L = digitalRead(LT_L);
  line_R = digitalRead(LT_R);
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
  {stopp();}  // 바꿔야함 
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

void back()
{
  digitalWrite(FL_D, LOW);           
  analogWrite(FL_S, 50);            

  digitalWrite(BL_D, LOW);            
  analogWrite(BL_S, 50);           

  digitalWrite(FR_D, LOW);            
  analogWrite(FR_S, 50);             

  digitalWrite(BR_D, LOW);          
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
      if(flag == 1) //x축방향
      {
        forward();
        delay(1000);
        RC_x -= 1;
      }
      
      else
      {
        turn_right();
        delay(3500);
        RC_x -= 1;
        flag = 1;
      }
    } 
     
    else if((randn == 1)&&(RC_y > 0)) //y축이동
    {
      if(flag == 1) //x축방향
      {
        turn_left();
        delay(3500);
        RC_y -= 1;
        flag = 0;
      }
  
      else // y축방향
      {
        forward();
        delay(1000);
        RC_y -= 1;
      }
    } 
  
  if((0 == RC_y)&&(0 == RC_x))  
  {
    goal = 0;
    destination();
    QR_x = 0;
    QR_y = 0;   
  }
}
void JunctionSelect() // 목적지까지 가는 함수
{
    randNumber = random(10,20);
    randn = randNumber % 2;

    if ((randn == 0) && (RC_x == QR_x - 48))
    {
      randn = 1; 
    }
    else if ((randn == 1) && (RC_y == QR_y - 48))
    {
      randn = 0; 
    }


  
    if((randn == 0) && (QR_x - 48 > RC_x)) //x축이동
    {
      if(flag == 1) //오른쪽을 보고있을때
      {
        forward();
        delay(1000);
        RC_x += 1;
      }
      else
      {
        turn_right();
        delay(3500);
        RC_x += 1;
        flag = 1;
      }
    }
     
    else if((randn == 1)&&(QR_y - 48 > RC_y)) //y축이동
    {
      if(flag == 1) //오른쪽보고있을떄
      {
        turn_left();
        delay(3500);
        RC_y += 1;
        flag = 0;
      }
  
      else
      {
        forward();
        delay(1000);
        RC_y += 1;
      }
    }
  
  if((QR_y - 48 == RC_y)&&(QR_x - 48 == RC_x))  
  {
    goal = 1;
    destination();
    Gi();
  }
}



void Gi() // 지게발 움직이는 함수
{
  if(gi==0)
  {
    digitalWrite(G_1, HIGH); 
    digitalWrite(G_3, LOW); 
              
    analogWrite(G_2, 120);            
    delay(3000); //확인필요
    gi = 1;
  }
  else
  {
    digitalWrite(G_1, LOW);
    digitalWrite(G_3, HIGH);
               
    analogWrite(G_2, 120);            
    delay(3000); //확인필요
    gi = 0;
  }
}

void destination() // 도착해서 대기공간까지 이동 함수
{
    if(flag == 1)
    {
        turn_right();
        delay(3870);
        flag = 0;
        back();
        delay(5000); // 대충 검은곳에 올라올 정도 
    }
    else
    {
        turn_left();
        delay(7740);
        //180도 돌아야함
        back();
        delay(5000);  // 출발지 도착
    }
    
}
