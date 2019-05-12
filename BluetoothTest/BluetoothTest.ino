bool shortCircuit = true ;
bool initAT = false ;
bool useSer1 = false ;
int cntr = 0 ;
bool ledOn = false ;

#include <SerialParser.h>

#define MASTER

volatile bool blinking = false ;

#ifndef MASTER
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
      String resp = "OK(0), You sent '" ;
      resp += cmd ;
      resp += "'" ;
      if ( !strcmp ( cmd, "quit" ) ) {
        Serial.end () ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "My commands:\r"+
        "\ton: blink on\r"+
        "\toff: blink off\r" ;
        return resp ;
      }
      return resp ;
    }
};

#else
// MASTER

class CommandInvokerDummy : public virtual AbstractCommandInvoker {

  public:
    String invoke ( char* cmd ) {
      String resp = "OK(1), You sent '" ;
      resp += cmd ;
      resp += "'" ;
      if ( !strcmp ( cmd, "quit" ) ) {
        Serial1.end () ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "You Asked me(1) ?\r" + "I'm stupid still but in future, I Will send You my command set and instruction\r" + "Bye" ;
        return resp ;
      }
      return resp ;
    }
};

class CommandInvokerOnOff : public virtual AbstractCommandInvoker {

  public:
    String invoke ( char* cmd ) {
      String resp = "OK(2), You sent '" ;
      resp += cmd ;
      resp += "'" ;
      if ( !strcmp ( cmd, "AT+" ) ) {
        digitalWrite ( 3, HIGH ) ;
        return "AT MODE ON" ;
      } else if ( !strcmp ( cmd, "AT-" ) ) {
        digitalWrite ( 3, LOW ) ;
        return "AT MODE OFF" ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "\tAT+ : ENABLE AT MODE\r" ;
        resp = resp + "\tAT- : DISABLE AT MODE" ;
        return resp ;
      }
      return resp ;
    }
};
#endif

#ifdef MASTER
//CommandInvokerDummy pd ;
CommandInvokerOnOff poo ;
SerialCommandParser1 test1(poo) ;
//SerialCommandParser2 test2(poo) ;
#else
CommandInvoker p ;
SerialCommandParser test(p) ;
#endif


void setup() {
  Serial.begin ( 9600 ) ;
  Serial.println ( "Hello" ) ;
#ifdef MASTER
  Serial1.begin ( 9600 ) ;
  Serial2.begin ( 9600 ) ;
  Serial1.println ( "Hello" ) ;
  Serial2.println ( "Hello" ) ;
//  pinMode ( 2, OUTPUT ) ;
  pinMode ( 3, OUTPUT ) ;
#endif
//  digitalWrite ( 2, HIGH ) ;//VCC
  digitalWrite ( 3, LOW ) ;// ENABLE AT-MODE
  //digitalWrite ( 4, HIGH ) ;//VCC
  pinMode ( 13, OUTPUT ) ;
}

void loop() {
  if ( initAT ) {
    //delay ( 100 ) ;
    digitalWrite ( 3, HIGH ) ;//EN
    initAT = false ;
  }
  if ( shortCircuit ) {
    if ( useSer1 ) {
      if ( Serial1.available () ) {
        int b = Serial1.read () ;
        Serial.write ( b ) ;
      }
      if ( Serial.available () ) {
        int b = Serial.read () ;
        Serial1.write ( b ) ;
      }
    } else {
      if ( Serial2.available () ) {
        int b = Serial2.read () ;
        Serial.write ( b ) ;
      }
      if ( Serial.available () ) {
        int b = Serial.read () ;
        Serial2.write ( b ) ;
      }
    }
#ifdef MASTER
    test1.check () ;
    //test2.check () ;
#endif
  } else {
    cntr++ ;
    if ( cntr > 32000 ) {
      cntr = 0 ;
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
#ifndef MASTER
    test.check () ;
#endif
  }
}
