void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT_PULLUP ) ;
  pinMode ( 3, INPUT_PULLUP ) ;
  pinMode ( 4, INPUT_PULLUP ) ;
  pinMode ( 5, INPUT ) ;
  pinMode ( 6, INPUT ) ;
  pinMode ( 7, INPUT ) ;
}

#define n 1500
byte buffer[n] ;

void loop() {
  long start = millis () ;
  for ( int i = 0 ; i < n ; i++ ) {
    /*
    int b = 0 ;
    
    if ( digitalRead ( 2 ) ) {
      b |= 0x01 ;
    }
    
    if ( digitalRead ( 3 ) ) {
      b |= 0x02 ;
    }
    
    if ( digitalRead ( 4 ) ) {
      b |= 0x04 ;
    }
    
    if ( digitalRead ( 5 ) ) {
      b |= 0x08 ;
    }
    if ( digitalRead ( 6 ) ) {
      b |= 0x10 ;
    }
    if ( digitalRead ( 7 ) ) {
      b |= 0x20 ;
    }
    */
    byte b = PIND ;
    
    buffer[i] = b >> 2 ;
  }
  long ms = millis () - start ;
  if ( Serial.available() > 0 ) {
    int ch = Serial.read () ;
    Serial.write ( 'B' ) ;
    Serial.write ( ms ) ;
    Serial.write ( ms >> 8 ) ;
    for ( int i = 0 ; i < n ; i++ )
      Serial.write ( buffer[i] ) ;
  }
  //Serial.println ( b ) ;
}
