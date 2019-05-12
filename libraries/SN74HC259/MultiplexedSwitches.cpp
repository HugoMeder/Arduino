#include "MultiplexedSwitches.h"
#include <arduino.h>

    MultiplexedSwitches::MultiplexedSwitches ( int s0, int s1, int s2, int clr_, int g_, int d ) {
      this->s0 = s0 ;
      this->s1 = s1 ;
      this->s2 = s2 ;
      this->clr_ = clr_ ;
      this->g_ = g_ ;
      this->d = d ;
      deactivate () ;
    }


    void MultiplexedSwitches::beginMuxMode () {
      activate () ;
      digitalWrite ( d, HIGH ) ;
      digitalWrite ( clr_, LOW ) ;
      digitalWrite ( g_, LOW ) ;
      addr = 0 ;
      setAddr () ;
    }

    void MultiplexedSwitches::beginLatchMode () {
      activate () ;
      digitalWrite ( clr_, LOW ) ;
      digitalWrite ( g_, HIGH ) ; // clear All bits
      delay ( 1 ) ;
      digitalWrite ( clr_, HIGH ) ; // memory mode
      addr = 0 ;
    }

    void MultiplexedSwitches::choosePin ( int pin ) {
      this->addr = pin ;
      setAddr () ;
    }

    void MultiplexedSwitches::setPin ( int pin, bool value ) {
      this->addr = pin ;
      setAddr () ;
      digitalWrite ( d, value ) ;
      delay ( 10 ) ;
      digitalWrite ( g_, LOW ) ; // set
      delay ( 10 ) ;
      digitalWrite ( g_, HIGH ) ; // memory mode
      delay ( 10 ) ;
    }

    void MultiplexedSwitches::activate () {
      pinMode ( s0, OUTPUT ) ;  
      pinMode ( s1, OUTPUT ) ;  
      pinMode ( s2, OUTPUT ) ;  
      pinMode ( clr_, OUTPUT ) ;  
      pinMode ( g_, OUTPUT ) ;  
      pinMode ( d, OUTPUT ) ;        
    }
    
    void MultiplexedSwitches::deactivate () {
      pinMode ( s0, INPUT ) ;  
      pinMode ( s1, INPUT ) ;  
      pinMode ( s2, INPUT ) ;  
      pinMode ( clr_, INPUT ) ;  
      pinMode ( g_, INPUT ) ;  
      pinMode ( d, INPUT ) ;        
    }
    
    void MultiplexedSwitches::setAddr () {
      digitalWrite ( s0, (addr & 1) != 0 ) ;  
      digitalWrite ( s1, (addr & 2) != 0 ) ;  
      digitalWrite ( s2, (addr & 4) != 0 ) ;  
    }
