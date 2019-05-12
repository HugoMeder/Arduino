


int nbaud = 7 ;

long int baud[7] = {
9600,
19200,
38400,
57600,
115200,
230400,
250000 } ;

int index = 0 ;
long nextTimeStamp = 0 ;

#define STEP 1000

void setup() {
  nextTimeStamp = millis ()+ STEP ;
  Serial.begin ( 9600 ) ;
  Serial.println ( "Hello" ) ;
  Serial1.begin ( 9600 ) ;
}

void test ( int index ) {
  Serial.print ( "baud=" ) ;
  Serial.println  ( baud[index] ) ;
  Serial1.begin ( baud[index] ) ;
  Serial.write ( 'A' ) ;
  Serial.write ( 'T' ) ;
  Serial.write ( '\r' ) ;
  Serial.write ( '\n' ) ;
}

void loop() {
  long now = millis () ;
  long diff = now-nextTimeStamp ;
  if ( diff >= 0 ) {
    nextTimeStamp = now+STEP ;
    test ( index ) ;
    index = (index + 1) % nbaud ;
  }
  if ( Serial1.available() ) {
    int b = Serial1.read () ;
    Serial.print ( "byte " ) ;
    Serial.println ( b ) ;
  }
}
