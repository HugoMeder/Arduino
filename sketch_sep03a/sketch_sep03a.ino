#include "EEPROM.h"

void setup() {
  Serial.begin ( 250000 ) ;
  long int len = EEPROM.length () ;
  Serial.println ( len ) ;
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
