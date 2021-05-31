// 좌회전 우회전이 자유자재로 되지 않음 수정이 필요
long randNumber;

  int QR = 33;
  int QR1_1 = 0;
  int QR2_1 = 0;
  int QR1 = QR / 10;
  int QR2 = QR % 10;
  int randn = 0;
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop() 
{
  if((QR1 >> 0 ) || (QR2 >> 0))
  {
  JunctionSelect();
  }
  else if((QR1 <= 0 ) || (QR2 <= 0))
  {
  JunctionSelect_comeback();
  } 
}


void JunctionSelect_comeback()
{
  if((QR1_1 >> 0 ) || (QR2_1 >> 0))
  {
    randNumber = random(10, 20);
    randn = randNumber % 2;
    
    if((randn == 0) && (QR1_1 >> 0))
    {
      Serial.println("도착 우회전");
      QR1_1 -= 1;
      Serial.print("남은 우회전: "); 
      Serial.println(QR1_1); 
      flag = 1;
    }
    else if((randn == 1) && (QR2_1 >> 0))
    {
      Serial.println("도착 직진"); 
      QR2_1 -= 1;
      Serial.print("남은 직진: "); 
      Serial.println(QR2_1); 
    }
  }
}

void JunctionSelect()
{
  if((QR1 >> 0 ) || (QR2 >> 0))
  {
    randNumber = random(10, 20);
    randn = randNumber % 2;

    if((randn == 0) && (QR1 >> 0))
    {
      Serial.println("출발 좌회전");
      QR1 -= 1;
      QR1_1 += 1;
      Serial.print("남은 좌회전: ");
      Serial.println(QR1);  
    }
    else if((randn == 1) && (QR2 >> 0))
    {
      Serial.println("출발 직진");
      QR2 -= 1;
      QR2_1 += 1;
      Serial.print("남은 직진: ");
      Serial.println(QR2); 
    }
  }
}
