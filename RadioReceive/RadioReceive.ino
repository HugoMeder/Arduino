#include "RF24.h"

//RF24 radio(3,10);//UNO
RF24 radio(3,53);//Mega


int intrs = 0 ;

void isr () {
	intrs++ ;
}

void setup()
{
	radio.begin() ;
	radio.setAutoAck( false ) ;
//	radio.setChannel( 0x50 ) ;
	//radio.enableDynamicPayloads() ;
	radio.disableDynamicPayloads() ;
	radio.setPayloadSize( 19 ) ;
	radio.setAddressWidth( 4 ) ;
	radio.setDataRate( RF24_2MBPS ) ;
	radio.setCRCLength(RF24_CRC_16 ) ;
	//radio.errNotify() ;
	radio.maskIRQ ( 0, 0, 0 ) ;
//	radio.maskIRQ ( 1, 1, 1 ) ;
	attachInterrupt( digitalPinToInterrupt ( 2 ), isr, CHANGE ) ;
	Serial.begin( 250000 ) ;
}

byte buffer[19] ;

uint64_t addr = 0xA963B87E ;
//	uint64_t addr = 0x7EB863A9 ;

void testChannel ( int i ) {
		radio.setChannel( i ) ;
		radio.startListening() ;
		radio.openReadingPipe( 1, addr ) ;
		delay ( 10 ) ;
		if ( radio.testCarrier() ) {
			Serial.print( "channel " ) ;
			Serial.print ( i ) ;
			int stat = radio.get_status() ;
			Serial.print( ", status " ) ;
			Serial.print ( stat ) ;
			Serial.print( ", intrs " ) ;
			Serial.println ( intrs ) ;
			if ( radio.available() ) {
				radio.read( buffer, 19 ) ;
				Serial.println ( "received!" ) ;
			}
		}
		radio.closeReadingPipe( 1 ) ;
		radio.stopListening() ;	
}

void loop()
{
	testChannel ( 0x50 ) ;
/*

	for ( int i = 0 ;  i < 120 ; i++ ) {
		testChannel ( i ) ;
	}	
	*/
}
