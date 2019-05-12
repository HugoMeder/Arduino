#include "Wire.h"

int ADS1115_addr = 0x48 ;
int MPL3115A2 = 0x48 ;

void setup() {
  Wire.begin () ;
  Serial.begin ( 9600 ) ;
}

void loop() {
    //Serial.println ( "write..." ) ;
    Wire.beginTransmission( MPL3115A2);
    Wire.write(0x26);
    Wire.write(0xB8);
    Wire.endTransmission();
    Wire.beginTransmission( MPL3115A2);
    Wire.write(0x13);
    Wire.write(0x07);
    Wire.endTransmission();
    Wire.beginTransmission( MPL3115A2);
    Wire.write(0x26);
    Wire.write(0xB9);
    Wire.endTransmission();
    bool ready = false ;
    while ( !ready ) {
      Wire.beginTransmission( MPL3115A2);
      Wire.write(0x00);
      Wire.endTransmission();
      Wire.requestFrom(MPL3115A2, (uint8_t)1);
      int stat = Wire.read () ;
      Serial.print ( "stat " ) ;
      Serial.println ( stat ) ;
      ready = (stat&0x08)== 0 ;       
    }
    Serial.println ( "Ready!" ) ;    
}
