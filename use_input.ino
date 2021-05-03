void setup() {
  Serial.begin(9600);

}

void loop() {
  putString();
}

void putString(){
  Serial.println("**입력시작**");
  while(1)
  {
    if(Serial.available()){
      String inString = Serial.readStringUntil('\n');
      Serial.println(inString);
      if (inString== "END") {
        Serial.println( " ** Method End ** ");
        break;
      }
    }
  }
}
