void setup(){
  Serial.begin(9600);     
}

void loop(){
  int n = 0;
  int str[];
  if (Serial.available() > 0) {  // 시리얼 버퍼에 데이터가 있으면 
    byte value = Serial.read();    
      for(n=0, n<10, n++)
      {
        str[n]=value;                
      }
  }
  for(n=0, n<10, n++)
  {
    Serial.println(str[n]);
  }
  break;
}
