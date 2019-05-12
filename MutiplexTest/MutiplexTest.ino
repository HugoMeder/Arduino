#include "MultiplexedSwitches.h"
/*
class MultiplexedSwitches {
  private:
    int s0, s1, s2, clr_, g_, d ;
    int addr ;
    
  public:
    MultiplexedSwitches ( int s0, int s1, int s2, int clr_, int g_, int d ) {
      this->s0 = s0 ;
      this->s1 = s1 ;
      this->s2 = s2 ;
      this->clr_ = clr_ ;
      this->g_ = g_ ;
      this->d = d ;
      deactivate () ;
    }


    void beginMuxMode () {
      activate () ;
      digitalWrite ( d, HIGH ) ;
      digitalWrite ( clr_, LOW ) ;
      digitalWrite ( g_, LOW ) ;
      addr = 0 ;
      setAddr () ;
    }

    void beginLatchMode () {
      activate () ;
      digitalWrite ( clr_, LOW ) ;
      digitalWrite ( g_, HIGH ) ; // clear All bits
      delay ( 1 ) ;
      digitalWrite ( clr_, HIGH ) ; // memory mode
      addr = 0 ;
    }

    void choosePin ( int pin ) {
      this->addr = pin ;
      setAddr () ;
    }

    void setPin ( int pin, boolean value ) {
      this->addr = pin ;
      setAddr () ;
      digitalWrite ( d, value ) ;
      digitalWrite ( g_, LOW ) ; // set
      //delay ( 1 ) ;
      digitalWrite ( g_, HIGH ) ; // memory mode
      //delay ( 1 ) ;
    }

private:
    void activate () {
      pinMode ( s0, OUTPUT ) ;  
      pinMode ( s1, OUTPUT ) ;  
      pinMode ( s2, OUTPUT ) ;  
      pinMode ( clr_, OUTPUT ) ;  
      pinMode ( g_, OUTPUT ) ;  
      pinMode ( d, OUTPUT ) ;        
    }
    
    void deactivate () {
      pinMode ( s0, INPUT ) ;  
      pinMode ( s1, INPUT ) ;  
      pinMode ( s2, INPUT ) ;  
      pinMode ( clr_, INPUT ) ;  
      pinMode ( g_, INPUT ) ;  
      pinMode ( d, INPUT ) ;        
    }
    
    void setAddr () {
      digitalWrite ( s0, (addr & 1) != 0 ) ;  
      digitalWrite ( s1, (addr & 2) != 0 ) ;  
      digitalWrite ( s2, (addr & 4) != 0 ) ;  
    }
};
*/
// MultiplexesSwitches sw ( 0, 2, 4, 1, 3, 5 ) ;
// MultiplexedSwitches sw ( 2, 4, 6, 3, 5, 7 ) ;
MultiplexedSwitches sw ( 0+8, 2+8, 4+8, 1+8, 3+8, 5+8 ) ;

void setup() {
  Serial.begin ( 9600 ) ;
  //sw.beginMuxMode () ;
  sw.beginLatchMode () ;
  
  //sw.setPin ( 6, true ) ;
  //sw.setPin ( 0, true ) ;
  
  /*
  sw.setPin ( 2, true ) ;
  sw.setPin ( 4, true ) ;
  */
  pinMode ( 13, OUTPUT ) ;
}

int addr = 0 ;
bool state = false ;

void muxMode () {
  sw.choosePin ( addr ) ;
  addr++ ;  
}

void latchMode () {
  sw.setPin ( 0, state ) ;
  sw.setPin ( 1, !state ) ;
  //sw.setPin ( 2, !state ) ;
  //sw.setPin ( 3, state ) ;
}

void loop() {
  latchMode () ;
  Serial.print ( "pin " ) ;
  Serial.println ( (addr & 7) ) ;  
  delay ( 1000 ) ;
  digitalWrite ( 13, state ) ;
  state = !state ;
}
