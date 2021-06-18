

int G_1 = 42; // 지게발
int G_3 = 46; // 지게발
int G_2 = 12; // pwm지게발
int n = 0;
void setup() {

  pinMode(G_1, OUTPUT);
  pinMode(G_2, OUTPUT);
  pinMode(G_3, OUTPUT);
  
  
 // analogWrite(G_2, 0);
}

void loop() {
down();
}

void down() // 반시계
{
      digitalWrite(G_1, HIGH); //LOW 푸는거 
    digitalWrite(G_3, LOW); //high 푸는거
    analogWrite(G_2, 70);          
}

void up() //시계
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
    delay(3000); //확인필요  
}
