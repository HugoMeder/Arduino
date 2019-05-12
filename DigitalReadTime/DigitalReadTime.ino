void setup() {
  Serial.begin ( 9600 ) ;
  pinMode ( 2, INPUT_PULLUP ) ;//1
  pinMode ( 3, INPUT_PULLUP ) ;//2
  pinMode ( 4, INPUT_PULLUP ) ;//3
  pinMode ( 5, INPUT ) ;//4
  pinMode ( 6, INPUT ) ;//5
  pinMode ( 7, INPUT ) ;//6
}

int val ;

bool readPin () {
  val = PIND ;  
}

void loop() {
  long start = millis ();
  long n = 100000 ; 
  for ( long i = 0 ;  i < n ; i++ ) {
/*    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
    val = digitalRead ( 4 ) ;
 */
/*    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
    digitalRead ( 4 ) ;
   */
/*    readPin () ;
    readPin () ;
    readPin () ;
    readPin () ;
    readPin () ;
    
    readPin () ;
    readPin () ;
    readPin () ;
    readPin () ;
    readPin () ;
   */
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
  
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
  val = PIND ;  
}

  long stop_ = millis () ;
  Serial.println ( (stop_-start) ) ;
  double millisecs = ((double)(stop_-start))/(10.0*n) ;
  Serial.print ( millisecs*1e3 ) ;
  Serial.println ( " microsecs per read" ) ;
}
