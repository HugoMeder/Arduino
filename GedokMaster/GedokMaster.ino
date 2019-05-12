
#include <SerialParser.h>

#define CMD_MODE 0
#define AT_MODE 1
#define TRANSP_MODE 2

#define STATE_PIN 2

volatile int mode = CMD_MODE ;

#define BT_UNKNOWN 0
#define BT_CONNECTED 1
#define BT_DISCONNECTED 2

class MotorControl {

  private:
    int clck ;
    int err ;
    int ccw ;
    int off_ ;
    int sleep ;
    int reset_ ;

  public:
    MotorControl ( int clck, int err, int ccw, int off_, int sleep, int reset ) {
      this->clck = clck ;
      this->err = err ;
      this->ccw = ccw ;
      this->off_ = off_ ;
      this->reset_ = reset ;
    }

    void setup () {
      pinMode ( clck, OUTPUT ) ;
      pinMode ( err, INPUT ) ;
      pinMode ( ccw, OUTPUT ) ;
      pinMode ( off_, OUTPUT ) ;
      pinMode ( sleep, OUTPUT ) ;
      pinMode ( reset_, OUTPUT ) ;
      digitalWrite ( clck, LOW ) ;
      digitalWrite ( reset_, HIGH ) ;
      digitalWrite ( ccw, HIGH ) ;// Clockwise rotation
      digitalWrite ( off_, LOW ) ;// Motor is OFF initially
      digitalWrite ( sleep, LOW ) ; // test sleep
//      digitalWrite ( sleep, HIGH ) ; // no sleep
    }

    void allOff () {
      //pinMode ( clck, OUTPUT ) ;
      pinMode ( err, INPUT ) ;
      pinMode ( ccw, INPUT ) ;
      pinMode ( off_, INPUT ) ;
      pinMode ( sleep, INPUT ) ;      
    }
    
    void allOn () {
      pinMode ( clck, OUTPUT ) ;
      pinMode ( err, INPUT ) ;
      pinMode ( ccw, OUTPUT ) ;
      pinMode ( off_, OUTPUT ) ;
      pinMode ( sleep, OUTPUT ) ;
    }
    
    void on () {
      digitalWrite ( off_, HIGH ) ;
    }

    void off () {
      digitalWrite ( off_, LOW ) ;
    }

    void step () {
      digitalWrite ( clck, HIGH ) ;
      delay ( 1 ) ;
      digitalWrite ( clck, LOW ) ;      
    }

    void reset () {
      digitalWrite ( reset_, LOW ) ;
      delay ( 1 ) ;
      digitalWrite ( reset_, HIGH ) ;      
    }

    bool error () {
      return !digitalRead ( err ) ;
    }
} ;

MotorControl motor ( 9, 10, 11, 12, 13, 8 ) ;

class StateWatch {
    int stateIndex ; // 0, 2: onTime, 1, 3 : offTIme
    bool firstCall = true ;
    long lastTime ;
    bool lastState ;
    int errorCnt = 0 ;
    long timing[4] ;
    bool connected ;
    long lastDecitionTime = -1 ;

  private:
    void error () {
      errorCnt++ ;
      firstCall = true ;
      lastDecitionTime = -1 ;
    }

    void printState () {
      String msg = "state " ;
      for ( int i = 0 ; i < 4 ; i++ ) {
        if ( i != 0 )
          msg += ", " ;
        msg += timing[i] ;
      }
      Serial.println ( msg ) ;
    }

  public:

    int getConnectionState () {
      if ( lastDecitionTime == -1 ) {
        return BT_UNKNOWN ;
      }
      long now = millis () ;
      long since = now - lastDecitionTime ;
      if ( since > 3000 ) {
        return BT_UNKNOWN ;
      } else {
        if ( connected ) {
          return BT_CONNECTED ;
        } else {
          return BT_DISCONNECTED ;
        }
      }
    }

    void newPinState ( long milliseconds, bool on ) {
      if ( firstCall ) {
        if ( on ) {
          firstCall = false ;
          lastTime = milliseconds ;
          lastState = on ;
          stateIndex = 0 ;
        }
        return ;
      }
      if ( on == lastState ) {
        error () ;
        return ;
      }
      long timeMs = milliseconds - lastTime ;
      lastTime = milliseconds ;
      timing[stateIndex] = timeMs ;
      stateIndex = (stateIndex + 1) % 4 ;
      lastState = on ;
      if ( stateIndex == 0 ) {
        connected = timing[1] > 2000 || timing[3] > 2000 ;
        lastDecitionTime = milliseconds ;
        //printState () ;
      }
    }
};

StateWatch sw ;


volatile int cntr = 0 ;

bool runMotor = false ;
int stepCntr = 0 ;

class MasterInvoker : public virtual AbstractCommandInvoker {

  private:

    bool motorOn = false ;
    
    String state () {
      /*
        String resp = "p[" ;
        resp += STATE_PIN ;
        resp += "] = " ;
        int val = digitalRead ( STATE_PIN ) ;
        resp += val ;
        resp += ", cntr = " ;
        resp += cntr ;
        return resp ;
      */
      int state = sw.getConnectionState () ;
      String connState ;
      switch ( state ) {
        case BT_CONNECTED :
          connState = "connected" ;
          break ;
        case BT_DISCONNECTED :
          connState = "disconnected" ;
          break ;
        default:
          connState = "connection state unknown" ;
          break ;
      }
      String motorState ;
      if ( motor.error () ){
        motorState = "motor: error " ;
      } else {
        motorState = "motor: OK" ;
      }
      return connState+"\r"+motorState ;
    }

    String command ( char* cmd ) {
      if ( !strcmp ( cmd, "at+" ) ) {
        digitalWrite ( 3, HIGH ) ;
        mode = AT_MODE ;
        return "AT MODE ON" ;
      } else if ( !strcmp ( cmd, "t" ) ) {
        mode = TRANSP_MODE ;
        return "TRANSPARENT MODE to UNO, type 'q' to quit" ;
      } else if ( !strcmp ( cmd, "s" ) ) {
        return state () ;
      } else if ( !strcmp ( cmd, "o" ) ) {
        runMotor = true ;
        stepCntr = 0 ;
        return "one turn.." ;
      } else if ( !strcmp ( cmd, "mon" ) ) {
        motor.on () ;
        return "motor on" ;
      } else if ( !strlen ( cmd ) ) {
        /*
        motorOn = !motorOn ;
        if ( motorOn ) {
          motor.on () ;
          return "motor on" ;          
        } else {
          motor.off () ;
          return "motor off" ;                    
        }
        */
        motor.step () ;
        return ( "" ) ;
      } else if ( !strcmp ( cmd, "moff" ) ) {
        motor.off () ;
        return "motor off" ;
      } else if ( !strcmp ( cmd, "r" ) ) {
        motor.reset () ;
        return "motor reset" ;
      } else if ( !strcmp ( cmd, "malloff" ) ) {
        motor.allOff () ;
        return "motor al pins highimpedance ( except clck)" ;
      } else if ( !strcmp ( cmd, "mallon" ) ) {
        motor.allOn () ;
        return "motor reset al pins ( except clck)" ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "\tat+ : enter AT-mode\r" ;
        resp = resp + "\tt : transparemnt mode to UNO(BT)\r" ;
        resp = resp + "\ts : status\r" ;
        resp = resp + "\tmon : motor on\r" ;
        resp = resp + "\tmoff : motor off\r" ;
        resp = resp + "\tmalloff : motor al pins highimpedance ( except clck)\r" ;
        resp = resp + "\tmallon : motor reset al pins ( except clck)\r" ;
//        resp = resp + "\t'' : toggle motor" ;
        resp = resp + "\t'' : motor step\r" ;
        resp = resp + "\tr : motor reset\r" ;
        resp = resp + "\to : motor one turn" ;
        return resp ;
      } else {
        return "?" ;
      }
    }

    String forward ( char* cmd ) {
      int l = strlen ( cmd ) ;
      for ( int i = 0 ; i < l ; i++ ) {
        Serial2.write ( cmd[i] ) ;
      }
      Serial2.write ( '\r' ) ;
      Serial2.write ( '\n' ) ;
      String resp = "(BT)>" ;
      resp += cmd ;
      return resp ;
    }


    String at_mode ( char* cmd ) {
      if ( !strcmp ( cmd, "at-" ) ) {
        digitalWrite ( 3, LOW ) ;
        mode = CMD_MODE ;
        return "AT MODE OFF" ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "\tat - Commands\r" ;
        resp = resp + "\tat- : TERMINATE AT MODE" ;
        return resp ;
      } else {
        return forward ( cmd ) ;
      }
    }

    String transparent_mode ( char* cmd ) {
      if ( !strcmp ( cmd, "q" ) ) {
        mode = CMD_MODE ;
        return "back to command mode" ;
      } else if ( !strcmp (cmd, "?" ) ) {
        String resp ;
        resp = resp + "\tat - Commands\r" ;
        resp = resp + "\tat- : TERMINATE AT MODE" ;
        return resp ;
      } else {
        return forward ( cmd ) ;
      }
    }

  public:
    String invoke ( char* cmd ) {

      switch ( mode ) {
        case CMD_MODE : return command ( cmd ) ;
        case AT_MODE : return at_mode ( cmd ) ;
        case TRANSP_MODE : return transparent_mode ( cmd ) ;
      }
    }
};


void statePinChanged () {
  long ms = millis () ;
  bool on = digitalRead ( STATE_PIN ) ;
  sw.newPinState ( ms, on ) ;
}

MasterInvoker poo ;
SerialCommandParser1 cmd(poo) ;

void setup() {
  Serial.begin ( 9600 ) ;
  Serial.println ( "Hello" ) ;
  Serial1.begin ( 9600 ) ;
  Serial2.begin ( 9600 ) ;
  Serial1.println ( "Hello" ) ;
  Serial2.println ( "Hello" ) ;
  pinMode ( 2, STATE_PIN ) ;//STATE
  attachInterrupt(digitalPinToInterrupt(STATE_PIN), statePinChanged, CHANGE );
  pinMode ( 3, OUTPUT ) ;
  digitalWrite ( 3, LOW ) ;// ENABLE AT-MODE
  motor.setup () ;
}

void loop() {
  if ( mode == AT_MODE || mode == TRANSP_MODE ) {
    if ( Serial2.available () ) {
      int b = Serial2.read () ;
      Serial1.write ( b ) ;
    }
  }
  cmd.check () ;
  if ( runMotor ) {
    stepCntr++ ;
    if ( stepCntr == 4*200 )
      runMotor = false ;
    motor.step () ;
    delay ( 100 ) ;
  }
}
