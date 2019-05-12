#include <Wire.h>
#include <Adafruit_ADS1015.h>

int ADS1115_addr = ADS1015_ADDRESS ;

Adafruit_ADS1115 ads (ADS1115_addr);

void setup() {
  Serial.begin ( 250000 ) ;
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV
  ads.begin () ;
  ads.readADC_Differential_0_1 () ;
  //ads.readADC_SingleEnded(0);
  //  Serial.begin ( 230400 ) ;
  //  Serial.begin ( 9600 ) ;
  pinMode ( 2, INPUT ) ;
}

long int cntr = 0 ;
#define ACK 0x6
#define SYN 0x16
#define STATE_ECHOSYNC 1
#define STATE_SYNCHED 2

int state = STATE_ECHOSYNC ;

void loop() {
  if ( false ) {
    cntr++ ;
    //Serial.write ( cntr >> 8 ) ;
    Serial.write ( cntr ) ;
  } else if ( false ) {
    if ( Serial.available() > 0 ) {
      cntr++ ;
      Serial.write ( cntr ) ;
      Serial.read () ;
    }
  } else {
    if ( Serial.available() > 0 ) {
      int b = Serial.read () ;
      if ( state == STATE_ECHOSYNC ) {
        if ( b == SYN ) {
          Serial.write ( SYN ) ;
        } else if ( b == ACK ) {
          Serial.write ( ACK ) ;
          cntr = 0 ;
          state = STATE_SYNCHED ;
        }
      } else {
        if ( b == ' ' ) {
          cntr++ ;
          Serial.write ( cntr >> 8 ) ;
          Serial.write ( cntr ) ;
        } else if ( b == 'v' ) {
          //          int v = ads.readADC_SingleEnded(0);
          //          int v = ads.readADC_Differential_0_1 () ;
          if ( false ) {
            //int v = ((Wire.read() << 8) | Wire.read());
            int v = ads.readADC_Differential_0_1 () ;
            Serial.write ( v >> 8 ) ;
            Serial.write ( v ) ;
          } else {
            /*
              Wire.beginTransmission(ADS1015_ADDRESS);
              Wire.write(ADS1015_REG_POINTER_CONVERT);
              Wire.endTransmission();
            */
            if ( true ) {
              if ( true ) {
                int micro = micros () ;
                Wire.requestFrom(ADS1115_addr, 2);
                Serial.write ( Wire.read() ) ;
                Serial.write ( Wire.read() ) ;
                Serial.write ( micro >> 8 ) ;
                Serial.write ( micro ) ;
              } else {
                int val = analogRead ( 3 ) ;
                Serial.write ( val >> 8 ) ;
                Serial.write ( val ) ;
              }
            } else {
              cntr++ ;
              Serial.write ( cntr >> 8 ) ;
              Serial.write ( cntr ) ;
            }
          }
        } else {
          state = STATE_ECHOSYNC ;
        }
      }
    }
  }
}
