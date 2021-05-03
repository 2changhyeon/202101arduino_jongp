void setup(){
  Serial.begin(9600);     
}

void loop(){
  int n = 0;
  int str[5]={0,0,0,0,0};
  if (Serial.available() > 0) {
     byte value = Serial.read();
        if(value != 0){
           str[n]=value;   
           Serial.println(str[n]);
              switch(str[n])
              {
          case 49: 
                str[n] = 50;
                break;
          case 50:
                str[n] = 49;
                break;
          default:
                break;              
              }
          Serial.println(str[n]);
          n++;
          if(n>5)
          {
            n=0;         
          }
      }
  }
}
