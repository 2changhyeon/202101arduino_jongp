
long randNumber;

  int QR = 23; // qr인풋에서 11을 더해줘야함.
  int QRsum = (QR % 10) + (QR / 10);
  int QR_x = QR / 10;
  int QR_y = QR % 10;
  
  int RC_x = 0;
  int RC_y = 0;
  
  int randn = 0;
  int flag = 0;
  int goal = 0;
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop() 
{
  if(goal == 0)
  {
  JunctionSelect();
  }
  else
  {
  Comeback();
  }
}


void Comeback()
{
  for(int i = 0; i< QRsum; i++) 
  {
    i++;
    randNumber = random(10,20);
    randn = randNumber % 2;
  
    if((randn == 0) && (RC_x > 0)) //x축이동
    {
      if(flag == 1) //왼쪽을 보고있을때
      {
        Serial.println("직진");
        RC_x -= 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
      else
      {
        Serial.println("좌회전");
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
        Serial.println("우회전");
        RC_y -= 1;
        flag = 0;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
  
      else
      {
        Serial.println("직진");
        RC_y -= 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    } 
  }
  if((0 == RC_y)&&(0 == RC_x))  
  {
    Serial.println("복귀완료");
  }
}
void JunctionSelect()
{
  for(int i = 0; i< QRsum; i++) 
  {
    i++;
    randNumber = random(10,20);
    randn = randNumber % 2;
  
    if((randn == 0) && (QR_x > RC_x)) //x축이동
    {
      if(flag == 1) //오른쪽을 보고있을때
      {
        Serial.println("직진");
        RC_x += 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
      else
      {
        Serial.println("우회전");
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
        Serial.println("좌회전");
        RC_y += 1;
        flag = 0;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
  
      else
      {
        Serial.println("직진");
        RC_y += 1;
        Serial.print(RC_x);
        Serial.println(RC_y);
      }
    }
  }
  if((QR_y == RC_y)&&(QR_x == RC_x))  
  {
    Serial.println("이동완료");
    goal = 1;
  }
}
