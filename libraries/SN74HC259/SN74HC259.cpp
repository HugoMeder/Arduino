#include <arduino.h>
#include "SN74HC259.h"

  SN74HC259::SN74HC259 ( int S0, int S1, int S2 ) {
    this->S0 = S0 ;
    this->S1 = S1 ;
    this->S2 = S2 ;
  }

  void SN74HC259::select ( int addr ) {
    bool b ;
    b = ( addr & 0x1 ) != 0 ;
    digitalWrite ( S0, b ) ;
    b = ( addr & 0x2 ) != 0 ;
    digitalWrite ( S1, b ) ;
    b = ( addr & 0x4 ) != 0 ;
    digitalWrite ( S2, b ) ;
  }
  
  void SN74HC259::begin () {
    pinMode ( S0, OUTPUT ) ;  
    pinMode ( S1, OUTPUT ) ;  
    pinMode ( S2, OUTPUT ) ;  
  }

  void SN74HC259::end () {
    pinMode ( S0, INPUT ) ;  
    pinMode ( S1, INPUT ) ;  
    pinMode ( S2, INPUT ) ;  
  }

/*
class SN74HC259 {
  private:
  
  int S0, S1, S2 ;
  
  public:
  SN74HC259 ( int S0, int S1, int S2 ) {
    this->S0 = S0 ;
    this->S1 = S1 ;
    this->S2 = S2 ;
  }

  void select ( int addr ) {
    bool b ;
    b = ( addr & 0x1 ) != 0 ;
    digitalWrite ( S0, b ) ;
    b = ( addr & 0x2 ) != 0 ;
    digitalWrite ( S1, b ) ;
    b = ( addr & 0x4 ) != 0 ;
    digitalWrite ( S2, b ) ;
  }
  void begin () {
    pinMode ( S0, OUTPUT ) ;  
    pinMode ( S1, OUTPUT ) ;  
    pinMode ( S2, OUTPUT ) ;  
  }

  void end () {
    pinMode ( S0, INPUT ) ;  
    pinMode ( S1, INPUT ) ;  
    pinMode ( S2, INPUT ) ;  
  }
};
*/