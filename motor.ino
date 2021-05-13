int spd = 120;
int PWM1 = 2;   //M1 speed (0~255)
int DIR1 = 22;  //M1 direction (HIGH or LOW)
int PWM2 = 3;   //M2 speed (0~255)
int DIR2 = 24;  //M2 direction (HIGH or LOW)
int PWM3 = 4;   //M3 speed (0~255)
int DIR3 = 26;  //M3 direction (HIGH or LOW)
int PWM4 = 5;   //M4 speed (0~255)
int DIR4 = 28;  //M4 direction (HIGH or LOW)
int n = 1000;
void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(28,OUTPUT);
  analogWrite(PWM1, 0);   //PWM Speed Control
  analogWrite(PWM2, 0);   //PWM Speed Control
  analogWrite(PWM3, 0);   //PWM Speed Control
  analogWrite(PWM4, 0);   //PWM Speed Control
}

void loop() { /* run over and over
    if (Serial.available()) {
    char c = Serial.read();
    if (c == '5') st();
    else if (c == '8') fw();
    else if (c == '2') bw();
    else if (c == '7') fl();
    else if (c == '9') fr();
    else if (c == '4') lf();
    else if (c == '6') rh();
    else if (c == '1') bl();
    else if (c == '3') br();
    else if (c == '/') lt();
    else if (c == '*') rt();
  
  }
  fw();
  delay(n);
  st();
  delay(500);
  bw();
  delay(n);
  st();
  delay(500);
  bw();
  delay(n);
  st();
  delay(500);
  fw();
  delay(n);
  st();
  delay(500);
  lf();
  delay(n);
  st();
  delay(500);
  rh();
  delay(n);
  st();
  delay(500);
  rh();
  delay(n);
  st();
  delay(500);
  lf();
  delay(n);
  st();
  delay(500);
  fl();
  delay(n);
  st();
  delay(500);
  br();
  delay(n);
  st();
  delay(500);
  br();
  delay(n);
  st();
  delay(500);
  fl();
  delay(n);
  st();
  delay(500);
  fr();
  delay(n);
  st();
  delay(500);
  bl();
  delay(n);
  st();
  delay(500);
  bl();
  delay(n);
  st();
  delay(500);
  fr();
  delay(n);
  st();
  delay(500);
  lt();
  delay(6000);
  st();
  delay(500);
  rt();
  delay(6000);
  st();
  delay(500);*/
  rt();
  delay(2250);
  st();
  delay(500);
}

void fw() {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void bw() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, HIGH);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void fl() {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);
  analogWrite(PWM1, 0);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, 0);   //PWM Speed Control
}

void fr() {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, 0);   //PWM Speed Control
  analogWrite(PWM3, 0);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void lf() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, HIGH);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void rh() {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, LOW );
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void bl() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, HIGH);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, 0);   //PWM Speed Control
  analogWrite(PWM3, 0);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void br() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, HIGH);
  analogWrite(PWM1, 0);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, 0);   //PWM Speed Control
}

void lt() {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, HIGH);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void rt() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);
  analogWrite(PWM1, spd);   //PWM Speed Control
  analogWrite(PWM2, spd);   //PWM Speed Control
  analogWrite(PWM3, spd);   //PWM Speed Control
  analogWrite(PWM4, spd);   //PWM Speed Control
}

void st() {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);
  analogWrite(PWM1, 0);   //PWM Speed Control
  analogWrite(PWM2, 0);   //PWM Speed Control
  analogWrite(PWM3, 0);   //PWM Speed Control
  analogWrite(PWM4, 0);   //PWM Speed Control
}
