#include <SoftwareSerial.h>
#include "MQ131.h"
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

MS5803 sensor(ADDRESS_HIGH);
SoftwareSerial BTSerial(6, 7); // SoftwareSerial(TX 6, RX 7)
byte buffer[1024]; // 데이터를 수신 받을 버퍼
int bufferPosition; // 버퍼에 데이타를 저장할 때 기록할 위치


int measurePin = 7; // 미세먼지 A핀 바꿔봐야됨
int ledPower = 2;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

float temperature_c, temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;
double base_altitude = 50.0;

int addr = 0x18; 
int day,hour,min,sec = 0; 
byte buff[2] = {0,0}; 
int status = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  BTSerial.begin(9600);
  sensor.reset();
  sensor.begin();

  Serial.println("Gamma Sensor Sensing Start");
  BTSerial.println("Gamma Sensor Sensing Start");
  
  //Read Firmware version  
  Gamma_Mod_Read(0xB4);  
  //Reset before operating the sensor  
  Gamma_Mod_Read(0xA0);  
  Serial.println("================================================");
  BTSerial.println("================================================");
  
  pressure_baseline = sensor.getPressure(ADC_4096);
 
  bufferPosition = 0;

 pinMode(ledPower, OUTPUT);


}

void loop() {

  //미세먼지 루프
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0 / 1024.0);

  dustDensity = 172 * calcVoltage - 100;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);

  Serial.print(" - Voltage: ");;
  Serial.print(calcVoltage);
  
  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity);
  Serial.println (" ㎍/m³ ");


  BTSerial.print("Raw Signal Value (0-1023): ");
  BTSerial.print(voMeasured);

  BTSerial.print(" - Voltage: ");;
  BTSerial.print(calcVoltage);
  
  BTSerial.print(" - Dust Density: ");
  BTSerial.print(dustDensity);
  BTSerial.println (" ㎍/m³ ");


  Serial.println("================================================");
  BTSerial.println("================================================");

  // Read temperature from the sensor in deg C. This operation takes about
  temperature_c = sensor.getTemperature(CELSIUS, ADC_512);

  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);

  pressure_relative = sealevel(pressure_abs, base_altitude);

  altitude_delta = altitude(pressure_abs , pressure_baseline);

  // Report values via UART
  Serial.print("Temperature C = ");
  Serial.println(temperature_c);

  Serial.print("Pressure abs (mbar)= ");
  Serial.println(pressure_abs);

  Serial.print("Pressure relative (mbar)= ");
  Serial.println(pressure_relative);

  Serial.print("Altitude change (m) = ");
  Serial.println(altitude_delta);

  Serial.println("================================================");//padding between outputs


  BTSerial.print("Temperature C = ");
  BTSerial.println(temperature_c);

  BTSerial.print("Pressure abs (mbar)= ");
  BTSerial.println(pressure_abs);

  BTSerial.print("Pressure relative (mbar)= ");
  BTSerial.println(pressure_relative);

  BTSerial.print("Altitude change (m) = ");
  BTSerial.println(altitude_delta);

  BTSerial.println("================================================");//padding between outputs

  Gamma_Mod_Read_Value();  
  Serial.println("================================================");
  BTSerial.println("================================================");

  delay(43000);
}


double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return (P / pow(1 - (A / 44330.0), 5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}


void Gamma_Mod_Read_Value(){  
  Gamma_Mod_Read(0xB0); // Read Status  
  /*Gamma_Mod_Read(0xB1); // Read Measuring Time  */
  Gamma_Mod_Read(0xB2); // Read Measuring Value (10min avg / 1min update)  
  Gamma_Mod_Read(0xB3); // Read Measuring Value (1min avg / 1min update) 
 }
 
void Gamma_Mod_Read(int cmd){  
  /* Begin Write Sequence */  
  Wire.beginTransmission(addr);  
  Wire.write(cmd);  
  Wire.endTransmission();  
  /* End Write Sequence */  
  delay(10);  
  /* Begin Read Sequence */  
  Wire.requestFrom(addr, 2);  
  byte i = 0; 
  while(Wire.available())  
  {    
    buff[i] = Wire.read();    
    i++;  
    }  
    /* End Read Sequence */    
    
    /* View Results */  
    Print_Result(cmd); 
    }
  
  void Print_Result(int cmd){  
    float value = 0.0f;  
    switch(cmd){    
      case 0xA0:      
      Serial.print("Reset Response\t\t\t");    
      BTSerial.print("Reset Response   ");  
      if(buff[0]== 1) {
        Serial.println("Reset Success.");   
        BTSerial.println("Reset Success.");}
      else{
        Serial.println("Reset Fail(Status - Ready)."); 
        BTSerial.println("Reset Fail(Status - Ready).");}
      break;
      
    case 0xB0:    
    {  
    Serial.print("Status\t\t\t\t");     
    BTSerial.print("Status    "); }
    switch(buff[0]){        
      case 0: Serial.println("Ready"); BTSerial.println("Ready"); break;
      case 1: Serial.println("10min Waiting"); BTSerial.println("10min Waiting"); break; 
      case 2: Serial.println("Normal"); BTSerial.println("Normal"); break;
      }      
      status = buff[0];      
      Serial.print("VIB Status\t\t\t");   
      BTSerial.print("VIB Status   ");   
      switch(buff[1]){        
        case 0: Serial.println("OFF"); BTSerial.println("OFF"); break;        
        case 1: Serial.println("ON"); BTSerial.println("ON"); break;      
        }      
        break;
      
    case 0xB2:      
    Serial.print("Measuring Value(10min avg)\t");  
    BTSerial.print("Measuring Value(10min avg)    ");      
    value = buff[0] + (float)buff[1]/100;      
    Serial.print(value);
    BTSerial.print(value); 
    Serial.println(" uSv/hr");      
    BTSerial.println(" uSv/hr");
    break;
    
    case 0xB3:      
    Serial.print("Measuring Value(1min avg)\t");
    BTSerial.print("Measuring Value(1min avg)      ");      
    value = buff[0] + (float)buff[1]/100;      
    Serial.print(value); 
    BTSerial.print(value);
    Serial.println(" uSv/hr"); 
    BTSerial.println(" uSv/hr");     
    break;  
    
    case 0xB4:      
    Serial.print("FW Version\t\t\t");  
    BTSerial.print("FW Version");    
    Serial.print("V");BTSerial.print("V"); Serial.print(buff[0]); BTSerial.print(buff[0]);     
    Serial.print(".");BTSerial.print("."); Serial.println(buff[1]); BTSerial.println(buff[1]);  
    break;          
    } 
    }
