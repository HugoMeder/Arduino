/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8

RF24 radio(3,10);//UNO
//RF24 radio(3,53);//Mega

//
// Channel info
//


//
// Setup
//

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(250000);
  printf_begin();
  //Serial.println(F("\n\rRF24/examples/scanner/"));

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setAutoAck(false);

  // Get into standby mode
  radio.startListening();
  radio.stopListening();

  //radio.printDetails();

  // Print out header, high then low digit
  /*
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x",i>>4);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    printf("%x",i&0xf);
    ++i;
  }
  Serial.println();
  */
}

//
// Loop
//


const int num_reps = 10;

const uint8_t num_channels = 126;
uint8_t values[num_channels];

void testLoop(void)
{
  // Clear measurement values
  memset(values,0,sizeof(values));

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // Select this channel
      radio.setChannel(i);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() ){
        ++values[i];
      }
    }
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%d ",i );
    for ( int j = 0 ; j < values[i] ; j++ )
      printf ( "*" ) ;
    printf ( "\n" ) ;
    ++i;
  }
}


void defaultLoop(void) {
	int n = 126 ;
	if ( Serial.available() ) {
		char ch = Serial.read() ;
		switch ( ch ) {
			case 'S' :
				Serial.write ( ch ) ;
				for ( int i = 0 ; i < n ; i++ ) {
					radio.setChannel(i);
					radio.startListening();
					delay( 1 );
					radio.stopListening();
					Serial.write ( radio.testCarrier() ) ;
				}
				break ;
			case 'N' :
				Serial.write ( ch ) ;
				Serial.write ( n ) ;
				break ;
			case 'L' :
				{
					Serial.write ( ch ) ;
					while ( !Serial.available() ) {
					}
					int val = Serial.read() ;
					radio.setPALevel( val ) ;
				}
				break ;
			case 'l' :
				Serial.write ( ch ) ;
				Serial.write ( (int)radio.getPALevel() ) ;
				break ;
			default:
				Serial.write ( '?' ) ;				
		}
	}
}

void loop(void) {
	defaultLoop () ;
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
