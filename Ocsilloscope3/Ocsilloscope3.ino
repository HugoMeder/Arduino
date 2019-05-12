#include <Wire.h>
#include "MultiplexedSwitches.h"

MultiplexedSwitches sw ( 0+8, 2+8, 4+8, 1+8, 3+8, 5+8 ) ;

int ADS1115_addr = 0x48 ;

// pulse generation
int pulsepin = -1 ;
int pulsePinMode = 0 ;
int pulseHighCount = 0 ;
int pulseLowCount = 0 ;
int pcntr = 0 ;
bool pstate = true ;

// message counter
int msgcntr = 0; 
int msgcntrmode = 0 ; // 1 : 8 bit, 2 : 16 bit

#define numChannels 2
int channelMode[numChannels] ;

#define HIGHPIN 2

#define ACK 0x6
#define NAK 21
#define SYN 0x16
#define STATE_ECHOSYNC 1
#define STATE_SYNCHED 2

int state = STATE_ECHOSYNC ;

#define maxbuflen 20

#define maxoutbuflen 20

class SerialHandler {

    int buff[maxbuflen] ;
    int bufflen = 0;

    int outbuff[maxoutbuflen] ;
    int outbufflen = 0;
    int outpos ;
    int cyclMsgCntr = 0 ;
  public:

    void write ( int b ) {
      if ( outbufflen < maxoutbuflen )
        outbuff[outbufflen++] = b ;
    }

    void readChannel ( byte addrOffset, byte mode ) {
      int addr = ADS1115_addr + addrOffset ;
      if ( mode == 1 ) {
        int micro = micros () ;
        Wire.requestFrom( addr, 2);
        write ( Wire.read() ) ;
        write ( Wire.read() ) ;
        write ( micro >> 8 ) ;
        write ( micro ) ;
      } else if ( mode == 2 ) {
        Wire.beginTransmission( addr);
        Wire.write(1); // CONFIG
        Wire.endTransmission();
        Wire.requestFrom( addr, 2);
        int h = Wire.read() ;
        int l = Wire.read() ;
        Wire.beginTransmission( addr);
        Wire.write(1); // CONFIG
        Wire.write(h|0x80 ) ;
        Wire.write(l) ;
        Wire.endTransmission();
        boolean ready = false ;
        for (int i = 0 ; i < 1000 ; i++ ) {
          Wire.requestFrom( addr, 2);
          int h = Wire.read() ;
          int l = Wire.read() ;
          if ( h & 0x80 ) {
            ready = true ;
            break ;
          }
        }
        int micro = micros () ;
        Wire.beginTransmission( addr);
        Wire.write(0); // CONVERT
        Wire.endTransmission();
        if ( ready ) {
          Wire.requestFrom( addr, 2);
          write ( Wire.read() ) ;
          write ( Wire.read() ) ;          
        } else {
          write ( 0 ) ;
          write ( 0 ) ;
        }
        write ( micro >> 8 ) ;
        write ( micro ) ;
      }
    }

    bool checkstate () {
      int b = buff[0] ;
      if ( b == 's' ) {
        if ( pulsePinMode == 3 ) {
          write ( pstate ) ;
        }
        for ( int i = 0 ; i < numChannels ; i++ ) {
          int mode = channelMode[i] ;
          readChannel ( i, mode ) ;
        }
        if ( msgcntrmode ) {
          if ( msgcntrmode == 1 ) {
            write ( msgcntr ) ;
          } else { // == 2
            write ( msgcntr >> 8 ) ;            
            write ( msgcntr ) ;            
          }
          msgcntr++ ;
        }
        if ( pulsePinMode == 3 ) {
          pcntr++ ;
          int cnt = pstate ? pulseHighCount : pulseLowCount ;
          if ( pcntr >= cnt ) {
            pstate = !pstate ;
            digitalWrite ( pulsepin, pstate ) ;
            pcntr = 0 ;
          }
        }
        return true ;
      } else if ( b == 'A'  ) {//write register
        if ( bufflen == 5 ) {
          int pt = 1 ;
          int ptr = buff[pt++] ;
          int addr = buff[pt++] ;
          Wire.beginTransmission(ADS1115_addr + addr);
          Wire.write((uint8_t)ptr);
          Wire.write((uint8_t)(buff[pt++]));
          Wire.write((uint8_t)(buff[pt++]));
          Wire.endTransmission();
          Wire.beginTransmission(ADS1115_addr + addr);
          Wire.write(0); // CONVERT
          Wire.endTransmission();
          write ( ACK ) ;
          return true ;
        } else {
          return false ;
        }
      } else if ( b == 'R'  ) {//read register
        if ( bufflen == 3 ) {
          int pt = 1 ;
          int ptr = buff[pt++] ;
          int addr = buff[pt++] ;
          Wire.beginTransmission(ADS1115_addr + addr);
          Wire.write((uint8_t)ptr);
          Wire.endTransmission();
          Wire.requestFrom( ADS1115_addr + addr, 2);
          write ( Wire.read() ) ;
          write ( Wire.read() ) ;
          Wire.beginTransmission(ADS1115_addr + addr);
          Wire.write(0); // CONVERT
          Wire.endTransmission();
          write ( ACK ) ;
          return true ;
        } else {
          return false ;
        }
      } else if ( b == 'S'  ) {
        if ( bufflen == 3 ) {
          int nr = buff[1] ;
          bool on = ( buff[2] != 0 ) ;
          sw.setPin ( nr, on ) ;
          write ( ACK ) ;
          return true ;
        } else {
          return false ;
        }
      } else if ( b == 'M'  ) {
        if ( bufflen == 2 ) {
          msgcntr = 0 ;
          msgcntrmode = buff[1] ;
          write ( ACK ) ;
          return true ;          
        } else {
          return false ;
        }
      } else if ( b == 'P' ) {
        if ( bufflen == 7 ) {
          int pt = 1 ;
          byte pulsePin_ = buff[pt++] ;
          pulsePinMode = buff[pt++] ;
          int hi_h = buff[pt++] &0xff ;
          int hi_l = buff[pt++] &0xff ;
          int lo_h = buff[pt++] &0xff ;
          int lo_l = buff[pt++] &0xff ;
          int hi = hi_h << 8 | hi_l ;
          int lo = lo_h << 8 | lo_l ;
          if ( pulsepin != -1 && pulsepin != pulsePin_ ) {
            pinMode ( pulsepin, INPUT ) ;
          }
          switch ( pulsePinMode ) {
            case 0 : 
              pinMode ( pulsePin_, INPUT ) ;
              break ;
            case 1 :
              pinMode ( pulsePin_, OUTPUT ) ;
              digitalWrite ( pulsePin_, LOW ) ;
              break ;
            case 2 :
              pinMode ( pulsePin_, OUTPUT ) ;
              digitalWrite ( pulsePin_, HIGH ) ;
              break ;
            case 3 :
              pstate = 0 ;
              pinMode ( pulsePin_, OUTPUT ) ;
              digitalWrite ( pulsePin_, pstate ) ;
              pcntr = 0 ;                      
          }
          pulsepin = pulsePin_ ;
          pulseHighCount = hi ;
          pulseLowCount = lo ;
          write ( ACK ) ;
          return true ;
        } else {
          return false ;
        }
      } else if ( b == 'C' ) {
        if ( bufflen == 3 ) {
          int pt = 1 ;
          int addr = buff[pt++] ;
          int mode = buff[pt++] ;
          channelMode[addr] = mode ;
          write ( ACK ) ;
          return true ;
        } else {
          return false ;
        }
      } else {
        state = STATE_ECHOSYNC ;
      }
    }

    void handle ( int b ) {
      if ( state == STATE_ECHOSYNC ) {
        if ( b == SYN ) {
          Serial.write ( SYN ) ;
        } else if ( b == ACK ) {
          Serial.write ( ACK ) ;
          state = STATE_SYNCHED ;
          bufflen = 0 ;
        }
      } else {
        //if ( outbufflen == 0  )
        //  write ( cyclMsgCntr++ ) ;
        if ( bufflen < maxbuflen ) {
          buff[bufflen++] = b ;
          if ( checkstate () ) {
            bufflen = 0 ;
          }
        } else {
          state = STATE_ECHOSYNC ;
        }
      }
    }

    void process () {
      if ( outbufflen > 0 ) {
        while ( outbufflen > 0 && Serial.availableForWrite() > 0 ) {
          Serial.write ( outbuff[outpos++] ) ;
          if ( outpos == outbufflen ) {
            outbufflen = 0 ;
            outpos = 0 ;
          }
        }
      } else if ( Serial.available() > 0 ) {
        int b = Serial.read () ;
        handle ( b ) ;
      }
    }
} ;

SerialHandler sh ;

void setup() {
  Serial.begin ( 250000 ) ;
  //Wire.setClock(400000L);
  Wire.begin () ;
  pinMode ( HIGHPIN, OUTPUT ) ;
  digitalWrite ( HIGHPIN, HIGH ) ;
    
  channelMode[0] = 1 ;//CONTINUOUS
  channelMode[1] = 1 ;//CONTINUOUS
  //  pinMode ( WPIN, OUTPUT ) ;
  /*
    int alert = !digitalRead ( ALERT ) ;
    if ( alert ) {
    digitalWrite ( RELAIS, LOW ) ;
    } else {
    digitalWrite ( RELAIS, HIGH ) ;
    attachInterrupt ( digitalPinToInterrupt(ALERT), handleAlert, CHANGE ) ;
    }
  */
  sw.beginLatchMode () ;
  //sw.setPin ( 5, true ) ;
}


void loop() {
  sh.process () ;
}
