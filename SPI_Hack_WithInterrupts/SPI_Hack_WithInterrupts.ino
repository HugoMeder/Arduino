#include "Arduino.h"
#include <avr/interrupt.h>
/*
#define SPDR 0x2E
#define SPSR 0x2D
#define SPCR 0x2C
*/

int numIntrs = 0 ;

class SPIMasterOrSlave ;

SPIMasterOrSlave* spi_i = NULL;

void ss_high_isr_ () ;

class SPIMasterOrSlave {

	public:
	int sspin ;
	byte *sendBuffer ;
	byte *receiveBuffers[2] ;
	int buffLength[2] ;
	byte *receiveBuffer ;

	int bufferLength ;
	int pos ;
	boolean isMaster ;
	boolean initialized ;
	boolean overflow ;
	boolean receiveBufferReady ;
	boolean ss_is_high ;
	boolean firstBufferActive ;
	public:

	SPIMasterOrSlave ( int bufferLength, int sspin = SS ) {
		this->sspin = sspin ;
		this->bufferLength = bufferLength ;
		sendBuffer = new byte[bufferLength] ;
		receiveBuffers[0] = new byte[bufferLength] ;
		receiveBuffers[1] = new byte[bufferLength] ;
		receiveBuffer = receiveBuffers[0] ;
	}


	void beginSlave () {
		initialized = true ;
		pinMode(SCK, INPUT);
		pinMode(MOSI, INPUT);
		pinMode(MISO, INPUT);
		pinMode(sspin, INPUT);
		isMaster = false ;
		spi_i = this ;
		//attachInterrupt ( digitalPinToInterrupt ( sspin ), ss_high_isr_, RISING ) ;
		receiveBufferReady = false ;
		ss_is_high = false ;
		SPCR = 0xC0 ;
		
	}
	
	byte* getReceiveBuffer ( int &buffLen ) {
		if ( !isMaster ) {
//			boolean ss_val = digitalRead ( sspin ) ;

//			boolean ss_val =  (PINB & 0x4) ; // UNO
			boolean ss_val =  (PINB & 0x1) ; // Mega

//			boolean ss_val = digitalRead ( sspin ) ;
			if ( !ss_val ) {
				ss_is_high = false ;
			} else if ( (pos > 0) && !ss_is_high ) {
				ss_high_isr() ;
			}
		}
		if ( !receiveBufferReady ) {
			buffLen = 0 ;
			return NULL ;
		} else {
			receiveBufferReady = false ;
			if ( firstBufferActive ) {
				buffLen = buffLength[1] ;
				return receiveBuffers[1] ;
			} else {
				buffLen = buffLength[0] ;
				return receiveBuffers[0] ;
			}
		}
	}
	
	/** do not call this function
	*/
	void ss_high_isr () {
		if ( pos > 0 ) {
			ss_is_high = true ;
			if ( firstBufferActive ) {
				buffLength[0] = pos ;
				receiveBuffer == receiveBuffers[1] ;
				firstBufferActive = false ;
			} else {
				buffLength[1] = pos ;
				receiveBuffer == receiveBuffers[0] ;
				firstBufferActive = true ;
			}
			pos = 0 ;
			receiveBufferReady = true ;
		}
	}
	
	/** do not call this function
	*/
	void isr () {
		numIntrs++ ;
		if ( isMaster ) {
			} else {
			if ( pos < bufferLength ) {
				receiveBuffer[pos] = SPDR ;
				SPDR = sendBuffer[pos] ;
				pos++ ;
			} else {
				overflow = true ;
			}
		}
	}
	
} ;

void ss_high_isr_ ()
{
	if ( spi_i ) {
		spi_i->ss_high_isr () ;
	}
}

ISR(SPI_STC_vect)
{
	if ( spi_i )
	spi_i->isr () ;
}

SPIMasterOrSlave spi ( 100 ) ;




byte SPItransfer(byte value) {
	SPDR = value;
	while (!(SPSR & (1 << SPIF)));
	delay(10);
	return SPDR;
}

void setup() {
	Serial.begin ( 250000 ) ;

	SPDR = -1 ;
	//attachGeneralInterrupt ( 18, isr ) ;
	//SPCR = 0x40 ;
	//SPCR = 0xC0 ;
	//spi.beginSlave () ;
	//ISR ( 18, isr ) ;
	/*
	Serial.print ( "SS " ) ;
	Serial.println ( SS ) ;
	Serial.print ( "SS intr " ) ;
	Serial.println ( digitalPinToInterrupt ( SS ) ) ;
	*/
	
			pinMode(SCK, INPUT);
			pinMode(MOSI, INPUT);
			pinMode(MISO, INPUT);
			pinMode(SS, INPUT);
		SPCR = 0x40 ;

}

int lastStat = -1 ;

void testAndPrint () {
	int stat = SPSR ;
	if ( stat != lastStat ) {
		Serial.println ( stat  ) ;
		if ( stat & 128  ) {
			int data = SPDR ;
			stat = SPSR ;
			while ( stat & 128 ) {
				data = SPDR ;
				stat = SPSR ;
			}
			Serial.print ( "data " ) ;
			Serial.println ( data ) ;
			Serial.print ( "stat " ) ;
			Serial.println ( stat  ) ;
		}
		lastStat = stat ;
	}
}

int received = 0 ;
int blocks = 0 ;

boolean ssHigh ;

#define n 20
int pos = n ;
int bitsPos ;
byte buffer[n] ;

void add ( byte b ) {
	if ( pos < n && pos > 0 ) {
		buffer[pos++] = b ;
	}
}

void rec () {
	byte stat = SPSR ;
	if ( stat & 128 ) {
		add ( SPDR ) ;
		ssHigh = false ;
		//received++ ;
		stat = SPSR ;
		while ( stat & 128 ) {
			add ( SPDR ) ;
			stat = SPSR ;
			//received++ ;
		}
	}
	//  if ( (PINB & 0x4) && !ssHigh ) { // UNO
	if ( (PINB & 0x1) && !ssHigh ) { // MEGA
		ssHigh = true ;
		//blocks++ ;
		if ( pos > 0 ) {
			buffer[bitsPos] = (pos - bitsPos - 1) * 8 ;
		}
		bitsPos = pos ;
		buffer[pos++] = -1 ;
	}
}

boolean doReceive = false ;

void add2 ( byte b ) {
	if ( pos < n && doReceive ) {
		buffer[pos++] = b ;
		if ( pos == 20 && (buffer[0]&0xff) == 0xB0 ) {
			doReceive = false ;
		}
	}
}


void rec2 () {
	byte stat = SPSR ;
	if ( stat & 128 ) {
		add2 ( SPDR ) ;
		ssHigh = false ;
		//received++ ;
		stat = SPSR ;
		while ( stat & 128 ) {
			add2 ( SPDR ) ;
			stat = SPSR ;
			//received++ ;
		}
	}
	//  if ( (PINB & 0x4) && !ssHigh ) { // UNO
	if ( (PINB & 0x1) && !ssHigh ) { // MEGA
		ssHigh = true ;
		pos = 0 ;
	}
}

void defaultLoop () {
	if ( Serial.available() ) {
		Serial.read () ;
		Serial.write ( 'X' ) ;
		pos = 0;
		while ( pos < n ) {
			rec () ;
		}
		Serial.write ( 0 ) ;
		Serial.write ( 0 ) ;
		Serial.write ( 0 ) ;
		Serial.write ( 0 ) ;
		Serial.write ( bitsPos ) ;
		Serial.write ( bitsPos >> 8 ) ;
		for ( int i = 0 ; i < bitsPos ; i++ )
		Serial.write ( buffer[i] ) ;
	}
}

void checkInterrupts () {
	
	Serial.print ( "num intrs " ) ;
	Serial.println ( numIntrs ) ;
	Serial.print ( "pos " ) ;
	Serial.println ( spi.pos ) ;
	
}

void checkSS () {
	
	Serial.print ( "SS " ) ;
	Serial.println ( digitalRead ( SS ) ) ;
	
}

void checkISP () {
	int len ;
	if ( spi.getReceiveBuffer( len ) ) {
		Serial.print ( "received " ) ;
		Serial.println ( len ) ;
	}
}

void handleISP () {
	int len ;
	byte* buff = spi.getReceiveBuffer( len ) ;
	if ( buff ) {
		if ( pos < n ) {
			buff[pos++] = len*8 ;
		}
		for ( int i = 0 ; i < len; i++ ) {
			if ( pos < n ) {
				buffer[pos++] = buff[i] ;
			}
		}
		//Serial.print( "pos " ) ;
		//Serial.println ( pos ) ;
	}
}

void loop() {
	if ( Serial.available() ) {
		Serial.read() ;
		pos = 0 ;
		doReceive = true ;
		while ( doReceive ) {
			rec2 () ;
			//Serial.print ( "pos " ) ;
			//Serial.println ( pos ) ;
		}
		Serial.write ( 'b' );
		for ( int i = 1 ; i < 20 ; i++ )
			Serial.write ( buffer[i] ) ;
	}
}
