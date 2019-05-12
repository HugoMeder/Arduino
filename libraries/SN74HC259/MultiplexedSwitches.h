#ifndef MULTIPLEXED_SWITCHES
#define MULTIPLEXED_SWITCHES

class MultiplexedSwitches {
  private:
    int s0, s1, s2, clr_, g_, d ;
    int addr ;
    
  public:
    MultiplexedSwitches ( int s0, int s1, int s2, int clr_, int g_, int d ) ;
    void beginMuxMode () ;
    void beginLatchMode () ;
    void choosePin ( int pin ) ;
    void setPin ( int pin, bool value ) ;
private:
    void activate () ;
    void deactivate () ;
    void setAddr () ;
};

#endif