#include "SN74HC259.h"

SN74HC259 latch ( 3, 4, 5 ) ;
int pinIndex = 0 ;

void setup() {
  latch.begin () ;
  Serial.begin ( 9600 ) ;
}

void loop() {
  Serial.print ( "addr " ) ;
  Serial.println ( pinIndex ) ;
  latch.select ( pinIndex ) ;
  pinIndex = ( pinIndex + 1 ) & 0x7 ;
  delay ( 1000 ) ;
}
