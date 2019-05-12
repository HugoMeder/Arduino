void setup() {
  Serial.begin ( 9600 ) ;
}

float fact = 0.85*0.22 ;

void loop() {
  float v = analogRead ( 0 )*5.0/1023.0 ;
  float i = v/fact ;
  Serial.print ( "v = " ) ;
  Serial.print ( v ) ;
  Serial.print ( ", i = " ) ;
  Serial.println ( i ) ;
  delay ( 1000 ) ;
}
