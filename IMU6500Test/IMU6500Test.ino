#include "Wire.h"

#define R_WHOAMI 117
#define R_POW_1 107

#define AccConf 28
#define ACCEL_XOUT 59
#define ACCEL_YOUT 61
#define ACCEL_ZOUT 63

#define GYRO_X_OUT 67
#define GYRO_Y_OUT 69
#define GYRO_Z_OUT 71

#define XA_OFFS 119
#define YA_OFFS 122
#define ZA_OFFS 125
#define FIFO_COUNT 114
#define USER_CTRL 106
#define FIFO_Enable 35
#define FIFO_DATA 116
#define SMPLRT_DIV 25
#define CONFIG 26
#define Interrupt_Status 58

#define addr 0x68


class IMU6500 {
public:
	unsigned char readRegister8 ( unsigned char regAddr ) {
		Wire.beginTransmission ( addr ) ;
		Wire.write ( regAddr ) ;
		Wire.endTransmission () ;
		Wire.requestFrom  ( addr, 1 ) ;
		return Wire.read () ;
	} ;

	int readRegister16 ( unsigned char regAddr ) {
		Wire.beginTransmission ( addr ) ;
		Wire.write ( regAddr ) ;
		Wire.endTransmission () ;
		Wire.requestFrom  ( addr, 2 ) ;
		unsigned int h = Wire.read () ;
		unsigned int l = Wire.read () ;
		return (h << 8 ) | l ;
	} ;
	
	void writeRegister8 ( unsigned char regAddr, unsigned char val ) {
		Wire.beginTransmission ( addr ) ;
		Wire.write ( regAddr ) ;
		Wire.write ( val ) ;
		Wire.endTransmission () ;
	} ;

	void writeRegister16 ( unsigned char regAddr, int val ) {
		Wire.beginTransmission ( addr ) ;
		Wire.write ( regAddr ) ;
		Wire.write ( val >> 8 ) ;
		Wire.write ( val ) ;
		Wire.endTransmission () ;
	} ;

	void enableFIFO () {
		writeRegister8 ( FIFO_Enable, 0x78 ) ;
		unsigned char reg = readRegister8 ( USER_CTRL ) ;
		writeRegister8 ( USER_CTRL, reg | 0x40 ) ;
	}
	
	void enableDMP () {
		writeRegister8 ( FIFO_Enable, 0x78 ) ;
		unsigned char reg = readRegister8 ( USER_CTRL ) ;
		writeRegister8 ( USER_CTRL, reg | 0xC0 ) ;
	}
	
	void reset () {
		unsigned char reg = readRegister8 ( R_POW_1 ) ;
		writeRegister8 ( R_POW_1, reg | 0x80 ) ;
	}
	
	void setDLPF ( unsigned char DLPF ) {
		unsigned char reg = readRegister8 ( CONFIG ) & 0xF8 ;
		writeRegister8 ( CONFIG, reg | DLPF ) ;
	}
	
	void readAcc ( int &x, int &y, int &z ) {
		x = readRegister16( ACCEL_XOUT ) ;
		y = readRegister16( ACCEL_YOUT ) ;
		z = readRegister16( ACCEL_ZOUT ) ;
	}

	void readGyro ( int &x, int &y, int &z ) {
		x = readRegister16( GYRO_X_OUT ) ;
		y = readRegister16( GYRO_Y_OUT ) ;
		z = readRegister16( GYRO_Z_OUT ) ;
	}
	
	boolean overflow () {
		unsigned char v = readRegister8( Interrupt_Status ) ;
		return 	( v & 0x10 ) != 0 ;
	}
	
	void enableFIFOIntr () {
		
	}
	
	void readAccAndGyroAndPrint () {
			int x, y, z ;
			readAcc ( x, y, z ) ;

			Serial.print ( "a " ) ;
			Serial.print ( x ) ;
			Serial.print ( " " ) ;
			Serial.print ( y ) ;
			Serial.print ( " " ) ;
			readGyro( x, y, z ) ;
			Serial.print ( z ) ;
			Serial.print ( "g " ) ;
			Serial.print ( x ) ;
			Serial.print ( " " ) ;
			Serial.print ( y ) ;
			Serial.print ( " " ) ;
			Serial.println ( z ) ;
	}
	
	int readFIFO6DOFAndPrint () {
		int rv = 0 ;
		int cnt = readRegister16( FIFO_COUNT ) ;
		while ( cnt > 24 ) {
			for ( int i = 0 ; i < 12 ; i++ ) {
				readRegister8( FIFO_DATA ) ;
			}
			rv += 12 ;
			cnt = readRegister16( FIFO_COUNT ) ;
		}
		if ( cnt < 12 )
			return rv ;
		unsigned int xh ;
		unsigned int xl ;
		unsigned int yh ;
		unsigned int yl ;
		unsigned int zh ;
		unsigned int zl ;
		int x ;
		int y ;
		int z ;
		xh = readRegister8( FIFO_DATA ) ;
		xl = readRegister8( FIFO_DATA ) ;
		yh = readRegister8( FIFO_DATA ) ;
		yl = readRegister8( FIFO_DATA ) ;
		zh = readRegister8( FIFO_DATA ) ;
		zl = readRegister8( FIFO_DATA ) ;
		x = ( xh << 8) | xl ;
		y = ( yh << 8) | yl ;
		z = ( zh << 8) | zl ;
		Serial.print( "a " ) ;
		Serial.print( x ) ;
		Serial.print( " " ) ;
		Serial.print( y ) ;
		Serial.print( " " ) ;
		Serial.print( z ) ;
		Serial.print( " g " ) ;
		xh = readRegister8( FIFO_DATA ) ;
		xl = readRegister8( FIFO_DATA ) ;
		yh = readRegister8( FIFO_DATA ) ;
		yl = readRegister8( FIFO_DATA ) ;
		zh = readRegister8( FIFO_DATA ) ;
		zl = readRegister8( FIFO_DATA ) ;
		x = ( xh << 8) | xl ;
		y = ( yh << 8) | yl ;
		z = ( zh << 8) | zl ;
		Serial.print( x ) ;
		Serial.print( " " ) ;
		Serial.print( y ) ;
		Serial.print( " " ) ;
		Serial.print( z ) ;
		Serial.println() ;
		rv += 12 ;
		return rv ;
	}
};

IMU6500 imu ;

void setup()
{
	Serial.begin ( 250000 ) ;
	Serial.println ( "startup..." ) ;
	Wire.begin () ;
	
	int val = imu.readRegister8( R_WHOAMI ) ;
	Serial.print ( "val " ) ;
	Serial.println ( val ) ;
	
	imu.reset() ;
	
	delay ( 100 ) ;
	
	imu.writeRegister8( AccConf, 1 << 3 ) ;
	/*
	imu.writeRegister16( XA_OFFS, 8000 ) ;
	imu.writeRegister16( YA_OFFS, 8000 ) ;
	imu.writeRegister16( ZA_OFFS, 8000 ) ;
	*/
	/*
	imu.writeRegister16( XA_OFFS, 0 ) ;
	imu.writeRegister16( YA_OFFS, 0 ) ;
	imu.writeRegister16( ZA_OFFS, 0 ) ;
	*/
	int axo = imu.readRegister16( XA_OFFS ) ;
	int ayo = imu.readRegister16( YA_OFFS ) ;
	int azo = imu.readRegister16( ZA_OFFS ) ;
	Serial.print ( "axo " ) ;
	Serial.println ( axo ) ;
	Serial.print ( "ayo " ) ;
	Serial.println ( ayo ) ;
	Serial.print ( "azo " ) ;
	Serial.println ( azo ) ;
	for ( int i = 0 ; i <= 126 ; i++ ) {
		int val = imu.readRegister8( i ) ;
		Serial.print ( "reg " ) ;
		Serial.print ( i ) ;
		Serial.print ( " = " ) ;
		Serial.println ( val ) ;			
	}
	
	imu.setDLPF ( 1 ) ;
	imu.writeRegister8( SMPLRT_DIV, 10-1 ) ;// 8-bits, arg is divider - 1 i.e 10-1 -> 100 hz 1200 bytes per second (6 16bit-values per sample)
//	imu.enableDMP() ;
	imu.enableFIFO() ;
}

int lastFIFOCnt = -1 ;


long milliStart = 0 ;
long bytes = 0 ;

void loop()
{

/*	int ax = imu.readRegister16( ACCEL_XOUT ) ;
	Serial.print ( "ax " ) ;
	Serial.println ( ax ) ;
*/
/*
	int fifocnt = imu.readRegister16( FIFO_COUNT ) ;
	if ( fifocnt != lastFIFOCnt ) {
		Serial.print( "fifocnt " ) ;
		Serial.println ( fifocnt ) ;
		lastFIFOCnt = fifocnt ;
	}
*/

/*
	int fifocnt = imu.readRegister16( FIFO_COUNT ) ;
	for ( int i = 0 ; i < fifocnt ; i++ ) {
		imu.readRegister8( FIFO_DATA ) ;		
	}	
*/

	int fifocnt = imu.readFIFO6DOFAndPrint () ;
	
	if ( fifocnt > 0 ) {
		long now = millis ();
		bytes += fifocnt ; 
		if ( milliStart == 0 ) {
			milliStart = now ;
		} else {
			double secs = (now-milliStart)/1000.0 ;
			double bps = bytes/secs ;
			Serial.print( "bps " ) ;
			Serial.println ( bps ) ;
		}
	}
	if ( fifocnt != lastFIFOCnt ) {
		Serial.print( "fifocnt " ) ;
		Serial.println ( fifocnt ) ;
		lastFIFOCnt = fifocnt ;
	}
	if ( imu.overflow() ) {
		Serial.println ( "Overflow" ) ;
	}
	
	//imu.readAccAndGyroAndPrint () ;
	/*
	int ax, ay, az ;
	imu.readAcc ( ax, ay, az ) ;

	Serial.print ( "ax " ) ;
	Serial.print ( ax ) ;
	Serial.print ( ", ay " ) ;
	Serial.print ( ay ) ;
	Serial.print ( ", az " ) ;
	Serial.println ( az ) ;
	*/
	
	/*
	float axd = ax ;
	float ayd = ay ;
	float azd = az ;
	int mi = 0x7fff ;
	float mid = mi ;
	float a = sqrt ( axd*axd+ayd*ayd+azd*azd )/mid ;
	int ai = (int)( a*1000.0 ) ;
	Serial.print ( "a " ) ;
	Serial.println ( ai ) ;
	*/
}
