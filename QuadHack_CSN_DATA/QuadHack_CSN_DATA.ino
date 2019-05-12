

#define n 1000
byte buffer[n] ;
volatile int pos = n ;
volatile boolean do_sample = false ;
#define dataPin 4

volatile int mask ;
int inbyte ;
int bits ;
int lenBytePos ;
volatile int clocks_outside = 0 ;
boolean lastClck = false ;

void clck_rising () {

  if ( pos < n ) {
    if ( do_sample ) {
      if ( digitalRead ( dataPin ) ) {
        inbyte |= mask ;
      }
      bits++ ;
      mask = mask >> 1 ;
      if ( mask == 0 ) {
        buffer[pos++] = inbyte ;
        mask = 0x80 ;
        inbyte = 0 ;
      }
    } else {
      clocks_outside++ ;
    }
  }
}

void clck () {

  if ( pos < n ) {
    boolean clck_ = digitalRead ( 2 ) ;
    if ( clck_ == lastClck )
      return  ;
    lastClck = clck_ ;
    if ( !clck_ )
      return ;
    if ( do_sample ) {
      if ( digitalRead ( dataPin ) ) {
        inbyte |= mask ;
      }
      bits++ ;
      mask = mask >> 1 ;
      if ( mask == 0 ) {
        buffer[pos++] = inbyte ;
        mask = 0x80 ;
        inbyte = 0 ;
      }
    } else {
      clocks_outside++ ;
    }
  }
}

int last_csn = true ;

void csn () {

  if ( pos < n ) {
    boolean csn_ = digitalRead ( 3 ) ;
    if ( last_csn == csn_ ) {
      return ;
    }
    last_csn == csn_;
    if ( csn_ ) {
      // end of data block
      if ( mask != 0x80 ) { // store last bits
        do_sample = false ;
        buffer[pos++] = inbyte ;
        buffer[lenBytePos] = bits ;
      }
    } else {
      // start of data block
      buffer[pos] = 0 ;
      lenBytePos = pos++ ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
    }
  }
}

void setup() {
  Serial.begin ( 250000 ) ;
  pinMode ( 2, INPUT_PULLUP ) ;//1
  pinMode ( 3, INPUT_PULLUP ) ;//2
  pinMode ( 4, INPUT_PULLUP ) ;//3
  pinMode ( 5, INPUT ) ;//4
  pinMode ( 6, INPUT ) ;//5
  pinMode ( 7, INPUT ) ;//6
  attachInterrupt ( digitalPinToInterrupt ( 2 ), clck_rising, RISING ) ;
  attachInterrupt ( digitalPinToInterrupt ( 3 ), csn, CHANGE ) ;
}

void loop() {
  if ( Serial.available() > 0 ) {
    int ch = Serial.read () ;
    if ( ch == 'A' ) {
      Serial.write ( 'X' ) ;
      mask = 0x80;
      bits = 0 ;
      clocks_outside = 0 ;
      pos = 0 ;
      while ( pos < n && clocks_outside < 10000 ) {
      }
      int p = pos;
      pos = n ;
      Serial.write ( clocks_outside ) ;
      Serial.write ( clocks_outside >> 8 ) ;
      Serial.write ( p ) ;
      Serial.write ( p >> 8 ) ;
      for ( int i = 0 ; i < p ; i++ )
        Serial.write ( buffer[i] ) ;
    }
  }
  //Serial.println ( b ) ;
}
