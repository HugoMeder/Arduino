#ifndef SERIALPARSER_H
#define SERIALPARSER_H

#ifdef ARDUINO_AVR_MEGA2560
	#define HASSER1
	#define HASSER2
#endif

class AbstractCommandInvoker {
public:
    virtual String invoke ( char* cmd ) = 0 ;  
};

class AbstractCommandParser {

#define maxblen 700

  private:
    char buf[maxblen] ;
    int pos = 0 ;
    int len = 0 ;
    bool recieving = true ;
    bool overfl = false ;
    AbstractCommandInvoker &invoker ;
    
  public:

    AbstractCommandParser ( AbstractCommandInvoker &inv )
    : invoker ( inv )
    {
    }
    
    void check () {
      for (;;) {
        if ( !checkByte () ) {
          return ;
        }
      }
    }

  private:
    bool checkByte () {
      if ( recieving ) {
        if ( canRead () ) {
          byte b = read () ;
          if ( b >= ' ' ) {
            if ( !overfl ) {
              buf[pos++] = b ;
              if ( pos == maxblen - 1 ) {
                overfl = true ;
                //Serial.println ( "overflow!" ) ;
              }
            }
          } else if ( b == '\r' ) {
            if ( overfl ) {
              overfl = false ;
              pos = 0 ;
            } else {
              buf[pos] = (byte)0 ;
              String resp = invoker.invoke ( buf ) ;
              int rl = resp.length () ;
			  if ( rl > 0 ) {
				if ( rl > maxblen - 1 ) {
					rl = maxblen - 1 ;
				}
				for ( int i = 0; i < rl ; i++ ) {
					buf[i] = resp.charAt ( i ) ;
				}
				buf[rl] = '\r';
				len = rl + 1 ;
				recieving = false ;
				pos = 0 ;				  
				}
            }
          }
          return true ;
        }
      } else { // transmit
        if ( canWrite () ) {
          write ( buf[pos++] ) ;
          if ( pos == len ) {
            pos = 0 ;
            recieving = true ;
          }
          return true ;
        }
      }
      return false ;
    }

  protected:
    virtual bool canWrite () = 0 ;
    virtual void write ( byte b ) = 0 ;
    virtual bool canRead () = 0 ;
    virtual byte read () = 0 ;
};

class SerialCommandParser : public virtual AbstractCommandParser {

public:

  SerialCommandParser (  AbstractCommandInvoker &invoker )
  : AbstractCommandParser ( invoker )
  {
  }
  protected:
    bool canWrite () {
      return Serial.availableForWrite() > 0 ;
    }
    void write ( byte b ) {
      Serial.write ( b ) ;
    }
    bool canRead () {
      return Serial.available() > 0 ;
    }
    byte read () {
      return Serial.read() ;
    }
  public:

} ;

#ifdef HASSER1 
class SerialCommandParser1 : public virtual AbstractCommandParser {

public:

  SerialCommandParser1 (  AbstractCommandInvoker &invoker )
  : AbstractCommandParser ( invoker )
  {
  }
  protected:
    bool canWrite () {
      return Serial1.availableForWrite() > 0 ;
    }
    void write ( byte b ) {
      Serial1.write ( b ) ;
    }
    bool canRead () {
      return Serial1.available() > 0 ;
    }
    byte read () {
      return Serial1.read() ;
    }
  public:

} ;
#endif

#ifdef HASSER2
class SerialCommandParser2 : public virtual AbstractCommandParser {

public:

  SerialCommandParser2 (  AbstractCommandInvoker &invoker )
  : AbstractCommandParser ( invoker )
  {
  }
  protected:
    bool canWrite () {
      return Serial2.availableForWrite() > 0 ;
    }
    void write ( byte b ) {
      Serial2.write ( b ) ;
    }
    bool canRead () {
      return Serial2.available() > 0 ;
    }

    byte read () {
      return Serial2.read() ;
    }
  public:

} ;
#endif

#endif // SERIALPARSER_H