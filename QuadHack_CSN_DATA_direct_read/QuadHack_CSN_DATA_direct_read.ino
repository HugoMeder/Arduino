

#define n 1000
byte buffer[n] ;
volatile int pos = n ;
volatile boolean do_sample = false ;
#define dataPin 4

volatile int mask ;
volatile int inbyte ;
volatile int bits ;
volatile int lenBytePos ;
volatile int clocks_outside = 0 ;
volatile boolean lastClck = false ;
volatile int redundant_CSN_intrs ;

void clck2 () {

  if ( pos < n ) {
    if ( do_sample ) {
      byte inreg = PIND ;
      //if ( (inreg & 0x04 ) != 0 ) 
      //  return ;
      if ( (inreg & 0x10 ) != 0 ) {// pin 4
        inbyte |= mask ;
      }
      bits++ ;
      mask = mask >> 1 ;
      if ( mask == 0 ) {
//        buffer[pos++] = inreg ;
        buffer[pos++] = inbyte ;
        mask = 0x80 ;
        inbyte = 0 ;
      }
    } else {
      clocks_outside++ ;
    }
  }
}

void clck1 () {

  if ( pos < n ) {
    if ( do_sample ) {
      inbyte |= mask ;
      bits++ ;
      mask = mask >> 1 ;
      if ( mask == 0 ) {
        buffer[pos++] = lenBytePos ;//bits + 1 ;//inbyte ;
        mask = 0x80 ;
        inbyte = 0 ;
      }
    } else {
      clocks_outside++ ;
    }
  }
}

void clck0 () {

  if ( pos < n ) {
    if ( do_sample ) {
      if ( digitalRead ( 4 ) ) {
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
      do_sample = false ;
      if ( mask != 0x80 ) { // store last bits
        buffer[pos++] = inbyte ;
      }
      buffer[lenBytePos] = bits ;
    } else {
      // start of data block
      /*
      buffer[pos++] = 0xFF ;
      lenBytePos = pos - 1 ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
      */
      lenBytePos = pos ;
      pos++ ;
      buffer[lenBytePos] = 0xFF ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
    }
  }
}

void csn2 () {

  if ( pos < n ) {
    byte inreg = PIND ;
    boolean csn_ = (inreg & 0x04) != 0 ;//O PIN2
    if ( csn_ ) {
      redundant_CSN_intrs++ ;
      return ;
    }
    last_csn == csn_;
    if ( csn_ ) {
      // end of data block
      if ( do_sample ) {
        do_sample = false ;
        if ( mask != 0x80 ) { // store last bits
          buffer[pos++] = inbyte ;
        }
        buffer[lenBytePos] = bits ;        
      }
    } else {
      if ( do_sample ) {
        do_sample = false ;
        if ( mask != 0x80 ) { // store last bits
          buffer[pos++] = inbyte ;
        }
        buffer[lenBytePos] = bits ;        
      }
      // start of data block
      /*
      buffer[pos++] = 0xFF ;
      lenBytePos = pos - 1 ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
      */
      lenBytePos = pos++ ;
      buffer[lenBytePos] = 0xFF ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
    }
  }
}

void csn_falling () {

  if ( pos < n ) {
    byte inreg = PIND ;
    boolean csn_ = (inreg & 0x04) != 0 ;//O PIN2
    if ( csn_ ) {
      redundant_CSN_intrs++ ;
      return ;
    }
    last_csn == csn_;
    if ( csn_ ) {
      // end of data block
      if ( do_sample ) {
        do_sample = false ;
        if ( mask != 0x80 ) { // store last bits
          buffer[pos++] = inbyte ;
        }
        buffer[lenBytePos] = bits ;        
      }
    } else {
      if ( do_sample ) {
        do_sample = false ;
        if ( mask != 0x80 ) { // store last bits
          buffer[pos++] = inbyte ;
        }
        buffer[lenBytePos] = bits ;        
      }
      // start of data block
      /*
      buffer[pos++] = 0xFF ;
      lenBytePos = pos - 1 ;
      mask = 0x80 ;
      bits = 0 ;
      do_sample = true ;
      */
      lenBytePos = pos++ ;
      buffer[lenBytePos] = 0xFF ;
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
  attachInterrupt ( digitalPinToInterrupt ( 3 ), clck2, RISING ) ;
//  attachInterrupt ( digitalPinToInterrupt ( 2 ), csn2 , CHANGE ) ;
  attachInterrupt ( digitalPinToInterrupt ( 2 ), csn_falling , FALLING ) ;
}

void loop() {
  if ( Serial.available() > 0 ) {
    int ch = Serial.read () ;
    if ( ch == 'A' ) {
      Serial.write ( 'X' ) ;
      mask = 0x80;
      bits = 0 ;
      clocks_outside = 0 ;
      redundant_CSN_intrs = 0 ;
      pos = 0 ;
      while ( pos < n && clocks_outside < 10000 ) {
      }
      int p = pos;
      pos = n ;
      Serial.write ( clocks_outside ) ;
      Serial.write ( clocks_outside >> 8 ) ;
      Serial.write ( redundant_CSN_intrs ) ;
      Serial.write ( redundant_CSN_intrs >> 8 ) ;
      Serial.write ( p ) ;
      Serial.write ( p >> 8 ) ;
      for ( int i = 0 ; i < p ; i++ )
        Serial.write ( buffer[i] ) ;
    }
  }
  //Serial.println ( b ) ;
}
