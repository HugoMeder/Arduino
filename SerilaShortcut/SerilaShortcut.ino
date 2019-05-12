void setup() {
  Serial.begin ( 9600 ) ;
  Serial1.begin ( 9600 ) ;
}

void loop() {
  if ( Serial1.available () ) {
    int b = Serial1.read () ;
    Serial.write ( b ) ;
  }
  if ( Serial.available () ) {
    int b = Serial.read () ;
    Serial1.write ( b ) ;
  }
}
