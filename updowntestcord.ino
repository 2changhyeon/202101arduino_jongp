
//지게발 승하강 과정에서 속도 차이 발생하는 것 막아야함

int G_1 = 42; // 지게발
int G_3 = 46; // 지게발
int G_2 = 12; // pwm지게발
int n = 0;
void setup() {

  pinMode(G_1, OUTPUT);
  pinMode(G_2, OUTPUT);
  pinMode(G_3, OUTPUT);
  Serial.begin(9600);
  
 // analogWrite(G_2, 0);
}

void loop() 
{
  if(Serial.available() > 0)
  {
    int a;
    a = Serial.read();
    Serial.println(a);
    if(a ==49)
    {
      aclock();
      Serial.println("down");
      
    }
    else if (a==50)
    {
      unclock();
      Serial.println("up");
      
    }
    else if(a ==51)
    {
      gstop();
      Serial.println("stop");
      
    }
  }
}

void unclock() // 반시계
{
    digitalWrite(G_1, HIGH); //LOW 푸는거 
    digitalWrite(G_3, LOW); //high 푸는거
    analogWrite(G_2, 70);          
}

void aclock() //시계
{
    digitalWrite(G_1, LOW); //LOW 푸는거 
    digitalWrite(G_3, HIGH); //high 푸는거
    analogWrite(G_2, 70);          
}

void gstop()
{
    digitalWrite(G_1, LOW); //LOW 푸는거 
    digitalWrite(G_3, HIGH); //high 푸는거
    analogWrite(G_2, 0);          
 //확인필요  
}
