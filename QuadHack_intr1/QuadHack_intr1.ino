

#define n 500
byte buffer[n] ;
int ts[n] ;
volatile int pos = n ;
volatile long last_micros ;
volatile boolean strobeFound ;

void intr () {
  if ( pos < n ) {
    int b = 0 ;

    if ( digitalRead ( 3 ) ) {
      b |= 0x01 ;
    }
    if ( digitalRead ( 4 ) ) {
      b |= 0x02 ;
    }
    if ( digitalRead ( 5 ) ) {
      b |= 0x04 ;
    }
    if ( digitalRead ( 6 ) ) {
      b |= 0x10 ;
    }
    if ( digitalRead ( 7 ) ) {
      b |= 0x20 ;
    }
    int ms = micros ();
    int delta = ms-last_micros ;
    last_micros = ms ;
    ts[pos] =  delta ;
    buffer[pos++] = b ;

    strobeFound = true ;
  }
}

void no_intr ( int delta ) {
  if ( pos < n ) {
    int b = 0x08 ;

    if ( digitalRead ( 3 ) ) {
      b |= 0x01 ;
    }
    if ( digitalRead ( 4 ) ) {
      b |= 0x02 ;
    }
    if ( digitalRead ( 5 ) ) {
      b |= 0x04 ;
    }
    if ( digitalRead ( 6 ) ) {
      b |= 0x10 ;
    }
    if ( digitalRead ( 7 ) ) {
      b |= 0x20 ;
    }
    ts[pos] =  delta ;
    buffer[pos++] = b ;
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
    Serial.write ( 'C' ) ;
    last_micros = micros () ;
    noInterrupts() ;
    pos = 0 ;
    strobeFound = false ;
    interrupts() ;
    while ( pos < n ) {
      int micro = micros () ;
      int delta = micro-last_micros ;
      if ( delta > 300 && strobeFound ) {
        noInterrupts() ;
        no_intr ( delta ) ;
        last_micros = micro ;
        interrupts() ;
      }
    }
    for ( int i = 0 ; i < n ; i++ ) {
      Serial.write ( buffer[i] ) ;
      int ts_ = ts[i] ;
      Serial.write ( ts_ ) ;
      Serial.write ( ts_ >> 8  ) ;
    }
  }
  //Serial.println ( b ) ;
}
