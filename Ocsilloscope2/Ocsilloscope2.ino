#include <Wire.h>

int ADS1115_addr = 0x48 ;

void setupADS1115 () {
  int config = 224 ;
  Wire.beginTransmission(ADS1115_addr);
  Wire.write((uint8_t)1); //config
  Wire.write((uint8_t)(config >> 8 ));
  Wire.write((uint8_t)( config));
  Wire.endTransmission();
  Wire.beginTransmission(ADS1115_addr);
  Wire.write(0); // CONVERT
  Wire.endTransmission();
}

#define ALERT 2
#define RELAIS 3
#define WPIN 5
#define WCOUNT 500
#define HIGHPIN 3 

int wcntr = 0 ;
bool wstate = true ;

void handleAlert () {
  int alert = !digitalRead ( ALERT ) ;
  digitalWrite ( RELAIS, LOW ) ;
}

void setup() {
  Serial.begin ( 250000 ) ;
  //Wire.setClock(400000L);
  Wire.begin () ;
  pinMode ( ALERT, INPUT ) ;
  pinMode ( HIGHPIN, OUTPUT ) ;
  digitalWrite ( HIGHPIN, HIGH ) ;
  pinMode ( WPIN, OUTPUT ) ;
  /*
  int alert = !digitalRead ( ALERT ) ;
  if ( alert ) {
    digitalWrite ( RELAIS, LOW ) ;
  } else {
    digitalWrite ( RELAIS, HIGH ) ;
    attachInterrupt ( digitalPinToInterrupt(ALERT), handleAlert, CHANGE ) ;
  }
  */
  //setupADS1115 () ;
}

#define ACK 0x6
#define NAK 21
#define SYN 0x16
#define STATE_ECHOSYNC 1
#define STATE_SYNCHED 2

int state = STATE_ECHOSYNC ;

#define maxbuflen 5

#define maxoutbuflen 9

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

    bool checkstate () {
      int b = buff[0] ;
      if ( b == 'L' ) {
        write( 'L' ) ;
        return true ;
      } else if ( b == 'M' ) {
        int micro = micros () ;
        //delay ( 10 ) ;
        cyclMsgCntr++ ;
        write ( cyclMsgCntr++ ) ;
        write ( micro >> 8 ) ;
        write ( micro ) ;
        return true ;
      } else if ( b == 'v' ) {
        int micro = micros () ;
        int addr = ADS1115_addr ;
        Wire.requestFrom( addr, 2);
        write ( Wire.read() ) ;
        write ( Wire.read() ) ;
        write ( micro >> 8 ) ;
        write ( micro ) ;
        return true ;
      } else if ( b == 'w' ) {
        doubleValue () ;
        return true ;
      } else if ( b == 'c' ) {
        doubleValue () ;
        wcntr++ ;
        write ( wstate ) ;
        if ( wcntr == WCOUNT ) {
          wcntr = 0 ;
          wstate = !wstate ;
          digitalWrite ( WPIN, wstate ) ;
        }
        return true ;
      } else if ( b == 'A'  ) {
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
      } else if ( b == 'B' ) {
        int pt = 1 ;
        int ptr = buff[pt++] ; ;
        /*int addr = Serial.read () ;
          Serial.read () ;
          Serial.read () ;
        */
        if ( ptr == ' ' )
          write ( ACK ) ;
        else
          write ( NAK ) ;
        return true ;
      } else {
        return false ;
      }
    }

    void doubleValue () {
      int addr = ADS1115_addr ;
      Wire.requestFrom( addr, 2);
      write ( Wire.read() ) ;
      write ( Wire.read() ) ;
      int micro = micros () ;
      write ( micro >> 8 ) ;
      write ( micro ) ;
      addr =  ADS1115_addr + 1 ;
      Wire.requestFrom( addr, 2);
      write ( Wire.read() ) ;
      write ( Wire.read() ) ;
      micro = micros () ;
      write ( micro >> 8 ) ;
      write ( micro ) ;
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

void loop() {
  sh.process () ;
}
