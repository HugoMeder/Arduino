#include "SPI.h"
/*
  #define SPDR 0x2E
  #define SPSR 0x2D
  #define SPCR 0x2C
*/

int numIntrs = 0 ;

void isr () {
  numIntrs++ ;
}

byte SPItransfer(byte value) {
  SPDR = value;
  while (!(SPSR & (1 << SPIF)));
  delay(10);
  return SPDR;
}

void setup() {
  Serial.begin ( 250000 ) ;
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, INPUT);
  pinMode(SS, INPUT);

  SPDR = -1 ;
  //SPCR = 0xC0 ;
  SPCR = 0x40 ;
  //attachInterrupt ( 18, isr, RISING ) ;
  //ISR ( 18, isr ) ;
}

int lastStat = -1 ;

void testAndPrint () {
  int stat = SPSR ;
  if ( stat != lastStat ) {
    Serial.println ( stat  ) ;
    if ( stat & 128  ) {
      int data = SPDR ;
      stat = SPSR ;
      while ( stat & 128 ) {
        data = SPDR ;
        stat = SPSR ;
      }
      Serial.print ( "data " ) ;
      Serial.println ( data ) ;
      Serial.print ( "stat " ) ;
      Serial.println ( stat  ) ;
    }
    lastStat = stat ;
  }
}

int received = 0 ;
int blocks = 0 ;

boolean ssHigh ;

#define n 1000
int pos = n ;
int bitsPos ;
byte buffer[n] ;

void add ( byte b ) {
  if ( pos < n && pos > 0 ) {
    buffer[pos++] = b ;
  }
}

void rec () {
  byte stat = SPSR ;
  if ( stat & 128 ) {
    add ( SPDR ) ;
    ssHigh = false ;
    //received++ ;
    stat = SPSR ;
    while ( stat & 128 ) {
      add ( SPDR ) ;
      stat = SPSR ;
      //received++ ;
    }
  }
  if ( (PINB & 0x4) && !ssHigh ) {
    ssHigh = true ;
    //blocks++ ;
    if ( pos > 0 ) {
      buffer[bitsPos] = (pos - bitsPos - 1) * 8 ;
    }
    bitsPos = pos ;
    buffer[pos++] = -1 ;
  }
}
void loop() {
  if ( Serial.available() ) {
    Serial.read () ;
    Serial.write ( 'X' ) ;
    pos = 0; 
    while ( pos < n ) {
      rec () ;
    }
    Serial.write ( 0 ) ;
    Serial.write ( 0 ) ;
    Serial.write ( 0 ) ;
    Serial.write ( 0 ) ;
    Serial.write ( bitsPos ) ;
    Serial.write ( bitsPos >> 8 ) ;
    for ( int i = 0 ; i < bitsPos ; i++ )
      Serial.write ( buffer[i] ) ;

  }
}
