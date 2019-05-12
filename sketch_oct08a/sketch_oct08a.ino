void setup() {
  Serial.begin ( 115200 ) ;
  pinMode ( 3, OUTPUT );
}

bool high = true ;

void loop() {
  double val = analogRead ( 0 )*5.0 / 1023.0 ;
  Serial.print ( "v = " ) ;
  if ( high ) {
    Serial.print ( "(h) " ) ;
  }
  else {
    Serial.print ( "(l) " ) ;
  }
  Serial.println ( val ) ;
  if ( high ) {
    //analogWrite ( 3, 125 ) ;
  }
  else {
    //analogWrite ( 3, 125 ) ;
  }
  high = !high ;
  digitalWrite ( 3, high ) ;
  delay ( 1000 ) ;
  //float in = analogRead ( 0 )*5.0/1023.0 ;
  //Serial.println ( in ) ;
  /*
  digitalWrite ( 3, high ) ;
  */
}
