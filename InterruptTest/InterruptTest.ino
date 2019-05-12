boolean last_csn ;
long redundant_CSN_intrs = 0 ;
long intrs = 0 ;

void csn2 () {

    byte inreg = PIND ;
    boolean csn_ = (inreg & 0x04) != 0 ;//O PIN2
    
    //boolean csn_ = digitalRead ( 2 ) ;
//    if ( last_csn == csn_ ) {
    if ( csn_ ) {
      redundant_CSN_intrs++ ;
      return ;
    }
    intrs++ ;
    
    last_csn = csn_;
}

void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT ) ;//1
  pinMode ( 3, INPUT ) ;//2
  pinMode ( 4, INPUT_PULLUP ) ;//3
  pinMode ( 5, INPUT ) ;//4
  pinMode ( 6, INPUT ) ;//5
  pinMode ( 7, INPUT ) ;//6
  attachInterrupt ( digitalPinToInterrupt ( 2 ), csn2 , FALLING ) ;
}

void loop() {
  Serial.print ( "intrs " ) ;
  Serial.println ( intrs ) ;
  Serial.print ( "ind " ) ;
  Serial.println ( PIND & 0x04 ) ;
  Serial.print ( "redundant " ) ;
  Serial.println ( redundant_CSN_intrs ) ;
  redundant_CSN_intrs = 0 ;
  intrs = 0 ;
  delay ( 1000 ) ;
}
