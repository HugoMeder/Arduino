

#define n 1500
byte buffer[n] ;
volatile int pos = n ;

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
    Serial.write ( 'A' ) ;
    pos = 0 ;
    while ( pos < n ) {
      
    }
    for ( int i = 0 ; i < n ; i++ )
      Serial.write ( buffer[i] ) ;
  }
  //Serial.println ( b ) ;
}
