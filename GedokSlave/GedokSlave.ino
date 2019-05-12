int cntr2 = 0 ;
bool ledOn = false ;

#include <SerialParser.h>
#define maxCntr 320 

#define DT 2
#define CLK 3

int state = 0 ;
int renum[4] = {0, 1, 3, 2} ;
volatile int cntr = 0 ;
bool clk ;
bool dt ;

void initState () {
  state = dt | (clk << 1);
  state = renum[state] ;
}

void update () {
  int s = dt | (clk << 1);
  s = renum[s] ;
  int diff = (s - state) & 3 ;
  state = s ;
  if ( diff == 1 ) {
    cntr++ ;
    if ( cntr == maxCntr+1 )
      cntr = -maxCntr+1 ;
  } else if ( diff == 3 ) {
    cntr-- ;
    if ( cntr == -maxCntr ) {
      cntr = maxCntr ;
    }
  } else {
    Serial.print ( "!!!!!! diff == " ) ;
    Serial.println ( diff ) ;
  }
  //Serial.print ( "s " ) ;
  //Serial.println ( s ) ;
}

void clkChanged () {
  int clk_ = digitalRead ( CLK ) ;
  if ( clk_ == clk ) {
    /*Serial.print ( "NO CHANGE!!!!!! CLK: " ) ;
      Serial.println ( clk ) ;
    */
  } else {
    clk = clk_ ;
    update () ;
  }
}

void dtChanged () {
  int dt_ = digitalRead ( DT ) ;
  if ( dt_ == dt ) {
    /*Serial.print ( "NO CHANGE!!!!!! DT: " ) ;
      Serial.println ( dt ) ;
    */
  } else {
    dt = dt_ ;
    update () ;
  }
}

volatile bool blinking = false ;

class CommandInvoker : public virtual AbstractCommandInvoker {

  public:
    String invoke ( char* cmd ) {
      if ( !strcmp ( cmd, "on" ) ) {
        blinking = true ;
        return "blink on" ;
      }
      if ( !strcmp ( cmd, "off" ) ) {
        blinking = false ;
        return "blink off";;
      }
      if ( !strcmp ( cmd, "z" ) ) {
        cntr = 0 ;
        return "angle = 0";;
      }
      if ( !strcmp ( cmd, "a" ) ) {
        String resp ="" ;
        resp += cntr ;
        return resp ;
      }
      if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "My commands (Slave):\r"+
        "\ton: blink on\r"+
        "\toff: blink off\r"+
        "\ta: angle\r"+
        "\tz: set angle ref\r" ;
        return resp ;
      }
      return "?" ;
    }
};

CommandInvoker p ;
SerialCommandParser test(p) ;


void setup() {
  Serial.begin ( 9600 ) ;
  Serial.println ( "Hello" ) ;
  pinMode ( 13, OUTPUT ) ;
  pinMode ( CLK, INPUT ) ;
  pinMode ( DT, INPUT ) ;
  digitalWrite ( CLK, HIGH ) ;
  digitalWrite ( DT, HIGH ) ;
  dt = digitalRead ( DT ) ;
  clk = digitalRead ( CLK ) ;
  initState () ;
  attachInterrupt ( digitalPinToInterrupt(CLK), clkChanged, CHANGE ) ;
  attachInterrupt ( digitalPinToInterrupt(DT), dtChanged, CHANGE ) ;
  //pinMode ( CLK, INPUT ) ;

}

void loop() {
    cntr2++ ;
    if ( cntr2 > 32000 ) {
      cntr2 = 0 ;
      if ( blinking ) {
        ledOn = !ledOn ;
        digitalWrite ( 13, ledOn ) ;
        /*
        if ( ledOn )
          Serial.println ( " LED on" ) ;
        else
          Serial.println ( " LED off" ) ;
        */
      }
    }
    test.check () ;
}
