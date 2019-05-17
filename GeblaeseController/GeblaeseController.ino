#include "DHT.h"
//#include <Wire.h>
//#include <Adafruit_MPL115A2.h>

//Adafruit_MPL115A2 mpl115a2;


#define DHTPIN_EXTERN 8
#define DHTPIN_INTERN 9
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
#define blnkPin 13
#define RelaisPin 3

bool blnkState = false ;
bool relaisState = false ;

//clock
unsigned long clockMillis = 0 ;
unsigned long clockSeconds = 0 ;

DHT dht_extern(DHTPIN_EXTERN, DHTTYPE);
DHT dht_intern(DHTPIN_INTERN, DHTTYPE);

void relaisOn () {
  if ( relaisState == true )
    return ;
  digitalWrite ( RelaisPin, true ) ;
  relaisState = true ;
}

void relaisOff () {
  if ( relaisState == false )
    return ;
  digitalWrite ( RelaisPin, false ) ;
  relaisState = false ;
}

void setup()
{
  Serial.begin(9600);
  Serial.println () ;
  Serial.println( "Ventilator-Control - Hello");

  pinMode ( blnkPin, OUTPUT ) ;
  pinMode ( RelaisPin, OUTPUT ) ;

  dht_extern.begin();
  dht_intern.begin();
  //relaisOn () ;
  //mpl115a2.begin();
}


float saettigungsDampfDruck ( float tempCelsius ) {
  return 6.112 * exp ( 17.62 * tempCelsius / (243.12 + tempCelsius) ) ;
}

float wasserDampfDruck ( float tempCelsius, float relHumidity ) {
  return saettigungsDampfDruck ( tempCelsius ) * relHumidity / 100.0 ;
}

void readHumTempSensorsAndReport () {
  float h = dht_extern.readHumidity(); //Luftfeuchte auslesen
  float t = dht_extern.readTemperature(); //Temperatur auslesen

  Serial.print("Data;");
  Serial.print(h);
  Serial.print(";");
  Serial.print(t);
  Serial.print(";");
  h = dht_intern.readHumidity(); //Luftfeuchte auslesen
  t = dht_intern.readTemperature(); //Temperatur auslesen
  Serial.print(h);
  Serial.print(";");
  Serial.print(t);
  Serial.println(";");
}

#define min_hum 40
#define max_hum 80
#define min_delta 10
#define max_delta 1

void getSensorValuenextControlSecssAndDecide () {
  float h_e = dht_extern.readHumidity(); //Luftfeuchte auslesen
  float t_e = dht_extern.readTemperature(); //Temperatur auslesen
  float ph2O_e = wasserDampfDruck ( t_e, h_e ) ;
  float h_i = dht_intern.readHumidity(); //Luftfeuchte auslesen
  float t_i = dht_intern.readTemperature(); //Temperatur auslesen
  float ph2O_i = wasserDampfDruck ( t_i, h_i ) ;
  float ph20_delta = ph2O_i-ph2O_e ;
  float critDelta = -1 ;
  bool doSwitchOn ;
  if ( h_i <= min_hum ) {
    doSwitchOn = false ;
  } else if ( h_i > max_hum ) {
    critDelta = max_delta ;
    doSwitchOn = (ph20_delta > critDelta) ;
  } else {
    float x = (h_i-min_hum)/(max_hum-min_hum) ;
    critDelta = max_delta*x+min_delta*(1.0-x) ;
    doSwitchOn = (ph20_delta > critDelta) ;
  }
  if ( doSwitchOn ) {
    relaisOn () ;
  } else {
    relaisOff () ;    
  }
  Serial.print ( "CTRL;" ) ;
  Serial.print(h_e);
  Serial.print(";");
  Serial.print(t_e);
  Serial.print(";");
  Serial.print(ph2O_e) ;
  Serial.print(";");
  Serial.print(h_i);
  Serial.print(";");
  Serial.print(t_i);
  Serial.print(";");
  Serial.print(ph2O_i) ;
  Serial.print(";");
  Serial.print(critDelta);
  Serial.print(";");
  Serial.print(ph20_delta
  );
  if ( doSwitchOn )
    Serial.print(";+;");
  else
    Serial.print(";-;");
  Serial.println () ;
}

bool doPoll = false ;
int pollDelaySeconds ;

#define blen 512

class CmdInterpreter {
  private:
    char buffer[blen + 1] ;
    int pos ;

  public :
    void nextChar ( char ch ) {
      if ( ch < ' ' ) {
        if ( ch == 13 ) {
          buffer[pos] = 0 ;
          pos = 0 ;
          if ( !interprete ( buffer ) ) {
            Serial.print ( "msg not nterpreted" ) ;
            Serial.print ( buffer ) ;
            Serial.println () ;
          }
        }
      } else {
        if ( pos >= blen ) {
          Serial.println ( "buffer overflow!" ) ;
        }
        buffer[pos++] = ch ;
      }
    }
  private:
    bool interprete ( char* msg ) {
      int len = strlen ( msg ) ;
      if ( len < 2 ) {
        return false ;
      }
      if ( msg[0] == 'P' && msg[1] == 'L' ) {
        int secs ;
        sscanf ( msg + 3, "%u", &secs ) ;
        pollDelaySeconds = secs ;
        doPoll = true ;
        /*
          Serial.print ( "poll " ) ;
          Serial.print ( secs ) ;
          Serial.println ( " secs" ) ;
        */
        return true ;
      } else if ( msg[0] == 'R' && msg[1] == '+' ) {
        relaisOn () ;
        return true ;
      } else if ( msg[0] == 'R' && msg[1] == '-' ) {
        relaisOff () ;
        return true ;
      } else if ( msg[0] == 'R' && msg[1] == '?' ) {
        if ( relaisState )
          Serial.println ( "R+" ) ;
        else
          Serial.println ( "R-" ) ;
        return true ;
      }
    }
};

/*
  void readPressureAndReport () {
  float pressureKPA = 0, temperatureC = 0;

  mpl115a2.getPT(&pressureKPA,&temperatureC);
  Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.print(" kPa  ");
  Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C both measured together");

  pressureKPA = mpl115a2.getPressure();
  Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");

  temperatureC = mpl115a2.getTemperature();
  Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C") ;
  };
*/
long nextControlSecs = 0 ;
#define controlCycleSecs 600
//#define controlCycleSecs 60

void callControlJob () {
  if ( clockSeconds > nextControlSecs ) {
    nextControlSecs += controlCycleSecs ;
    getSensorValuenextControlSecssAndDecide () ;
  }
}

long nextSecond = 0 ;

void callPollingJob () {
  if ( doPoll ) {
    if ( clockSeconds >= nextSecond ) {
      nextSecond += pollDelaySeconds ;
      readHumTempSensorsAndReport () ;
      //readPressureAndReport () ;
    }
  } else {
    nextSecond = clockSeconds ;
  }
}

void callJobs () {
  callPollingJob () ;
  callControlJob () ;
}

unsigned long lastMillis = 0 ;

void newTime () {
  unsigned long now = millis () ;
  unsigned long diff = now - lastMillis ;
  if ( diff == 0 )
    return ;
  unsigned long ncm = clockMillis + diff ;
  if ( ncm > 1000 ) {
    unsigned long incSecs = ncm / 1000L ;
    ncm = ncm - incSecs * 1000L ;
    clockSeconds += incSecs ;
  }
  clockMillis = ncm ;
  lastMillis = now ;
  callJobs () ;
}

CmdInterpreter cmd ;

void loop() {

  //delay ( 1000 ) ;
  /*blnkState = !blnkState ;
    digitalWrite ( blnkPin, blnkState ) ;
  */
  while ( Serial.available () ) {
    int ch = Serial.read () ;
    cmd.nextChar ( ch ) ;
  }
  newTime () ;
}
