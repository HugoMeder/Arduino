void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT ) ;
  pinMode ( 3, INPUT ) ;
  pinMode ( 4, INPUT ) ;
  pinMode ( 5, INPUT ) ;
  pinMode ( 6, INPUT ) ;
  pinMode ( 7, INPUT ) ;
}

#define n 1500
byte buffer[n] ;

void loop() {
  if ( Serial.available() > 0 ) {
    int index = Serial.read () + 1 ;
    for ( int i = 0 ; i < n ; i++ ) {
      buffer[i] = digitalRead ( index ) ;
    }
    Serial.write ( 'A' ) ;
    for ( int i = 0 ; i < n ; i++ )
      Serial.write ( buffer[i] ) ;
  }
}
