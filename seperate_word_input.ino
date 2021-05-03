void setup(){
  Serial.begin(9600);     
}

void loop(){
  if (Serial.available() > 0) {  // 시리얼 버퍼에 데이터가 있으면 
    byte value = Serial.read();    
    Serial.write(value);
    Serial.print(", ");
    Serial.println(value);               
  }
}
