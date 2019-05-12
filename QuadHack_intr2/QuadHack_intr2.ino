

#define n 1000
byte buffer[n] ;
volatile int pos = n ;
int mask ;
int b ;

void intr () {
  if ( pos < n ) {
    
    if ( digitalRead ( 3 ) ) {
      b |= mask ;
    }
    mask = mask << 1 ;
    if ( mask == 0x100 ) {
      buffer[pos++] = b ;
      mask = 1 ;
      b = 0 ;      
    }
  }
}

void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT ) ;//1
  pinMode ( 3, INPUT ) ;//2
  pinMode ( 4, INPUT ) ;//3
  pinMode ( 5, INPUT ) ;//4
  pinMode ( 6, INPUT ) ;//5
  pinMode ( 7, INPUT ) ;//6
  attachInterrupt ( digitalPinToInterrupt ( 2 ), intr, RISING ) ;
}

void loop() {
  if ( Serial.available() > 0 ) {
    int ch = Serial.read () ;
    Serial.write ( 'D' ) ;
    mask = 1 ;
    pos = 0 ;
    interrupts() ;
    while ( pos < n ) {
    }
    Serial.write ( n ) ;
    Serial.write ( n >> 8 ) ;
    for ( int i = 0 ; i < n ; i++ ) {
      Serial.write ( buffer[i] ) ;
    }
  }
  //Serial.println ( b ) ;
}
