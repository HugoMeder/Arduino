#include <SPI.h>

byte seq1[] =
{ 5, 0x30, 0x7e, 0xb8, 0x63, 0xa9 } ;
byte seq2[] =
{ 5, 0x2a, 0x7e, 0xb8, 0x63, 0xa9} ;
byte seq3[] =
{ 2, 0x21, 0x0} ;
byte seq4[] =
{ 2, 0x22, 0x1};
byte seq5[] =
{ 2, 0x26, 0x27};
byte seq6[] =
{ 2, 0x31, 0x13};
byte seq7[] =
{ 2, 0x3d, 0x1};
byte seq8[] =
{ 5, 0x30, 0x7e, 0xb8, 0x63, 0xa9 } ;
byte seq9[] =
{ 5, 0x2a, 0x7e, 0xb8, 0x63, 0xa9} ;
byte seq10[] =
{ 20, 0xb0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte seq11[] =
{ 2, 0x23, 0x2};
byte seq12[] =
{ 2, 0x21, 0x0};
byte seq13[] =
{ 2, 0x22, 0x1};
byte seq14[] =
{ 2, 0x3d, 0x1};
byte seq15[] =
{ 2, 0x24, 0x0};
byte seq16[] =
{ 2, 0x25, 0x50};
byte seq17[] =
{ 2, 0x26, 0x27};
byte seq18[] =
{ 2, 0x23, 0x2};
byte seq19[] =
{ 2, 0x20, 0x7e};// no interrupts
//{ 2, 0x20, 0xe};
byte seq20[] =
{ 5, 0x30, 0x1, 0x2, 0x15, 0xa7};
byte seq21[] =
{ 2, 0x26, 0x27};
byte seq22[] =
{ 2, 0x3d, 0x1};
byte seq23[] =
{ 2, 0x25, 0x3};

byte* initseq[] =
{seq1,seq2,seq3,seq4,seq5,seq6,seq7,seq8,seq9,seq10,seq11,seq12,seq13,seq14,seq15,seq16,seq17,seq18,seq19,seq20,seq21,seq22,seq23} ;
	
	
byte send1[] =
{ 2, 0x0, 0x0} ;  // read config
byte send2[] =
{ 2, 0xe1, 0xe} ; // flush TX FIFO
byte getstat[] =
{ 2, 0x7, 0x0} ;
byte clearIntr[] =
{ 2, 0x27, 0x20} ;
byte getconf[] =
{ 2, 0x0, 0x0} ;

#define initseqlen 23

SPISettings spiSettings ;
//SPIClass spi (); 

#define iobuflen 20
byte iobuff[iobuflen] ;

volatile int isrcnt = 0 ;

void isr () {
	isrcnt++ ;
}

void transact (byte* buff, int len ) {
	digitalWrite ( SS, LOW ) ;
	delayMicroseconds ( 100 ) ;
	SPI.transfer( buff, len ) ;
	delayMicroseconds ( 100 ) ;
	digitalWrite ( SS, HIGH ) ;
}

byte const_transact (byte* buff, int len ) {
	memcpy ( iobuff, buff, len ) ;
	transact ( iobuff, len ) ;
	return iobuff[0] ;
}

byte sendBuf ( byte *ptr ) {
	int len = *ptr++ ;
	return const_transact ( ptr, len ) ;
}

void initRadio () {
	for ( int i = 0 ; i < initseqlen; i++  ) {
		sendBuf ( initseq[i] ) ;
	}	
}

boolean doTransmit = false ;
#define datalen 19
byte databuf[datalen+1] ;

#define CE 3

void setup0()
{
	Serial.begin ( 250000 ) ;
	//Serial.println ( "setup start" ) ;
	
	//attachInterrupt( digitalPinToInterrupt ( 2 ), isr, CHANGE ) ;
	SPI.begin( ) ;
	pinMode ( SS, OUTPUT ) ;
	pinMode ( CE, OUTPUT ) ;
	pinMode ( MISO, INPUT ) ;
	pinMode ( 2, INPUT ) ;
	digitalWrite ( CE, LOW ) ;
	digitalWrite ( SS, HIGH ) ;
	if ( false ) {
		Serial.print( "SS " ) ;
		Serial.println ( SS ) ;
		Serial.print( "MOSI " ) ;
		Serial.println ( MOSI ) ;
		Serial.print( "MISO " ) ;
		Serial.println ( MISO ) ;
		Serial.print( "SCK " ) ;
		Serial.println ( SCK ) ;
	}
	initRadio () ;
	for ( int i = 0; i < datalen ; i++ ) {
		databuf[i] = 0 ;
	}
	//Serial.println ( "setup done" ) ;
	digitalWrite ( CE, LOW ) ;
	
	
}

int loops ;
int stat ;

void printBits ( int byte ) {
	int mask = 0x80 ;
	while ( mask ) {
		Serial.print ( (byte & mask) ? '1' : '0' ) ;
		mask = mask >> 1 ;
	}
}

void printStat ( ) {
	Serial.print  ( "stat " ) ;
	printBits ( stat ) ;
	Serial.println () ;
}


void sendBuffer () {
	
	sendBuf ( send1 ) ;
	sendBuf ( send2 ) ;
	stat = sendBuf ( clearIntr ) ;
	//printStat () ;
	databuf[0] = 0xb0 ;
	stat = const_transact ( databuf, 20 ) ;
	//printStat () ;
	//stat = sendBuf ( getconf ) ;
	//stat = iobuff[1] &0xff ;
	while ( (stat & 0x20) == 0 ) {
		loops++ ;
		stat = sendBuf ( getstat ) ;
	}
	//printStat () ;
}


void defaultLoop () {
	if ( doTransmit )
		sendBuffer () ;
	if ( Serial.available() ) {
		char ch = (char) Serial.read() ;
		switch ( ch ) {
			case 'e' : {
				Serial.write ( ch ) ;
				doTransmit = true ;
				break ;
			}
			case 'd' : {
				Serial.write ( ch ) ;
				doTransmit = false ;
				break ;
			}
			case 'b' : {
				Serial.write ( ch ) ;
				for ( int i = 0 ; i < 5 ; i++ ) {
					while ( !Serial.available() )
					delayMicroseconds( 10 ) ;
					databuf[i+1] = Serial.read() ;
				}
				break ;
			}
		}
	}	
}

void setup () {
	setup0() ;
	/*
	Serial.print  ( "isrcnt " ) ;
	Serial.println ( isrcnt ) ;
	*/
	/*
	printStat () ;
	sendBuffer () ;
	printStat () ;
	*/
}

void loop()
{
	//defaultLoop () ;
	
	
	if ( true ) {		
		//sendBuffer ( ) ;
		stat = sendBuf( getstat ) ;
		Serial.print  ( "stat " ) ;
		printBits ( stat ) ;
		Serial.println () ;
		Serial.print  ( "isrcnt " ) ;
		Serial.println ( isrcnt ) ;
	}
	
	//Serial.print  ( "Send Buffer, loops " ) ;
	//Serial.println ( loops ) ;
/*	Serial.print  ( "stat " ) ;
	Serial.println ( stat ) ;
	Serial.print  ( "isrcnt " ) ;
	Serial.println ( isrcnt ) ;
	*/
	//sendBuffer () ;
	//defaultLoop () ;
	
}
