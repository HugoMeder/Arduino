void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT ) ;
  pinMode ( 3, INPUT ) ;
  pinMode ( 4, INPUT ) ;
  pinMode ( 5, INPUT ) ;
  pinMode ( 6, INPUT ) ;
  pinMode ( 7, INPUT ) ;
}

void loop() {
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
  Serial.write ( b ) ;
  //Serial.println ( b ) ;
}
