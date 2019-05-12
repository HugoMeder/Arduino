#ifndef SN74HC259_H
#define SN74HC259_H

class SN74HC259 {
  private:
  int S0, S1, S2 ;
  
  public:
  SN74HC259 ( int S0, int S1, int S2 ) ;
  void select ( int addr ) ;
  void begin () ;
  void end () ;
};
#endif