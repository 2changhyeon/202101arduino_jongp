#include <Wire.h>
#include <sensorbar.h>
#include <usbhid.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

#define FL_D 22
#define FL_S 2
#define BL_D 26
#define BL_S 3
#define FR_D 30
#define FR_S 6
#define BR_D 34
#define BR_S 5

#define G_1 10
#define G_2 11

#define LT_L 38
#define LT_R 40


//ground 제대로 물려주기.
int speed = 100;
int j = 1;
int up_down = 0;
int line_R = 0;
int line_L = 0;


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
uint8_t MyParser::KeyToAscii(bool upper, uint8_t mod, uint8_t key) {
  
  // Letters
  if (VALUE_WITHIN(key, 0x04, 0x1d)) {
    if (upper) return (key - 4 + 'A');
    else return (key - 4 + 'a');
  }

  // Numbers
  else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
    return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
  }

  return 0;
}

//qr
void MyParser::OnKeyScanned(bool upper, uint8_t mod, uint8_t key) {
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
void MyParser::OnScanFinished() {
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
  pinMode(G_2, OUTPUT);

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
  getLT();
  LT();
  Usb.Task();
  
  if((line_L == 1)&&(line_R == 1)&&(LTresult == "11111111"))
  {
    Usb.Task();  
    if(QR_x != 0 && QR_y != 0)
    {
      //여기서 이제 뒤로 이동해서 상하차 해야됨.
      //상하차 과정 코드로 작성해야함
       test(); 
       delay(500);
    }
  }
  
  else //if((line_L == 0)&&(line_R == 0))
  {
    driving();    
  }
  
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
}

void LT()
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


void up()
{
  if(up_down== 0)
  {
    digitalWrite(G_1, LOW);     //모터A+ LOW
    analogWrite(G_2, 120);  //모터A-의 속력을 PWM 출력
    up_down = 1;
  }
  else
  {
    digitalWrite(G_1, HIGH);     //모터A+ LOW
    analogWrite(G_2, 120);  //모터A-의 속력을 PWM 출력
    up_down = 0;
  }
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
    if(flag == 1)
    {
        turn_left();
        delay(3500);
        flag = 0;
        //이제 여기 상하차 과정추가
        stopp();
        delay(100000);
    }
    else
    {
      //이제 여기 상하차 과정추가
      stopp();
      delay(100000);
    }
  }
}
void JunctionSelect()
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
    if(flag == 1)
    {
        turn_left();
        delay(3500);
        flag = 0;
        //이제 여기 상하차 과정추가
        stopp();
        delay(100000);
    }
    else
    {
      //이제 여기 상하차 과정추가
      if(goal == 1)
      {
        G_down();
        goal = 0;
      }
      else
      {
        G_up();
      }
      stopp();
      delay(100000);
    }
  }
}

void G_up()
{

  digitalWrite(G_1, HIGH);           
  analogWrite(G_2, 255);            
  delay(3000); //확인필요
}



void G_down()
{

  digitalWrite(G_1, LOW);           
  analogWrite(G_2, 255);            
  delay(3000); //확인필요
}
