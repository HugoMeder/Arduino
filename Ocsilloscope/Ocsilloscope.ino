#include <Wire.h>

int ADS1115_addr = 0x48 ;

void setupADS1115 () {
  int config = 224 ;
  Wire.beginTransmission(ADS1115_addr);
  Wire.write((uint8_t)1); //config
  Wire.write((uint8_t)(config >> 8 ));
  Wire.write((uint8_t)( config));
  Wire.endTransmission();
  Wire.beginTransmission(ADS1115_addr);
  Wire.write(0); // CONVERT
  Wire.endTransmission();
}
void setup() {
  Serial.begin ( 250000 ) ;
  Wire.begin () ;
  pinMode ( 2, INPUT ) ;
  setupADS1115 () ;
}

#define ACK 0x6
#define NAK 21
#define SYN 0x16
#define STATE_ECHOSYNC 1
#define STATE_SYNCHED 2

int state = STATE_ECHOSYNC ;

void loop() {
  if ( Serial.available() > 0 ) {
    int b = Serial.read () ;
    if ( state == STATE_ECHOSYNC ) {
      if ( b == SYN ) {
        Serial.write ( SYN ) ;
      } else if ( b == ACK ) {
        Serial.write ( ACK ) ;
        state = STATE_SYNCHED ;
      }
    } else {
      if ( b == 'v' ) {
        int micro = micros () ;
        Wire.requestFrom(ADS1115_addr, 2);
        Serial.write ( Wire.read() ) ;
        Serial.write ( Wire.read() ) ;
        Serial.write ( micro >> 8 ) ;
        Serial.write ( micro ) ;
      } else if ( b == 'A' ) {
        int ptr = Serial.read () ;
        int addr = Serial.read () ;
        Wire.beginTransmission(ADS1115_addr + addr);
        Wire.write((uint8_t)ptr);
        Wire.write((uint8_t)(Serial.read()));
        Wire.write((uint8_t)(Serial.read()));
        Wire.endTransmission();
        Wire.beginTransmission(ADS1115_addr + addr);
        Wire.write(0); // CONVERT
        Wire.endTransmission();
        Serial.write ( ACK ) ;
      } else if ( b == 'B' ) {
        int ptr = Serial.read () ;
        /*int addr = Serial.read () ;
        Serial.read () ;
        Serial.read () ;
        */
        if ( ptr == ' ' )
          Serial.write ( ACK ) ;
        else
          Serial.write ( NAK ) ;
      } else {
        state = STATE_ECHOSYNC ;
      }
    }
  }
}
