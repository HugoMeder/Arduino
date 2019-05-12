#include "DHT.h"
//#include <Wire.h>
//#include <Adafruit_MPL115A2.h>

//Adafruit_MPL115A2 mpl115a2;


#define DHTPIN 9
#define DHTPIN2 8
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
#define blnkPin 13
#define RelaisPin 3

bool blnkState = false ;
bool relaisState = false ;

//clock
unsigned long clockMillis = 0 ;
unsigned long clockSeconds = 0 ;

DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  Serial.println () ;
  Serial.println( "Ventilator-Control - Hello");

  pinMode ( blnkPin, OUTPUT ) ;
  pinMode ( RelaisPin, OUTPUT ) ;
  dht.begin();
  dht2.begin();
  //mpl115a2.begin();
}

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

float saettigungsDampfDruck ( float tempCelsius ) {
    return 6.112*exp ( 17.62*tempCelsius/(243.12+tempCelsius) ) ;
}

float wasserDampfDruck ( float tempCelsius, float relHumidity ) {
    return saettigungsDampfDruck ( tempCelsius )*relHumidity/100.0 ;
}

void readHumTempSensorsAndReport () {
  float h = dht.readHumidity(); //Luftfeuchte auslesen
  float t = dht.readTemperature(); //Temperatur auslesen

  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
/*  if (isnan(t) || isnan(h))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else */
  {
//    Serial.print("Luftfeuchte: ");
    Serial.print("Data;");
    Serial.print(h);
//    Serial.print("Temperatur: ");
    Serial.print(";");
//    Serial.print(t);
    Serial.print(t);
 //   Serial.println(" C");
    /*float p_w = wasserDampfDruck ( t, h ) ;
    float p_w_s = saettigungsDampfDruck ( t ) ;
    Serial.print("Wasser-Dampfdruck " );
    Serial.print(p_w);
    Serial.println(" hPa");
    Serial.print("Saettigungs-Dampfdruck 2 " );
    Serial.print(p_w_s);
    Serial.println(" hPa");
    */
  }

  h = dht2.readHumidity(); //Luftfeuchte auslesen
  t = dht2.readTemperature(); //Temperatur auslesen

  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
  /*if (isnan(t) || isnan(h))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else*/
  {
//    Serial.print("Luftfeuchte: ");
    Serial.print(";");
    Serial.print(h);
    Serial.print(";");
//    Serial.print("Temperatur: ");
//    Serial.print(t);
    Serial.print(t);
    Serial.println(";");
//    Serial.println(" C");
/*    float p_w = wasserDampfDruck ( t, h ) ;
    float p_w_s = saettigungsDampfDruck ( t ) ;
    Serial.print("Wasser-Dampfdruck 2 " );
    Serial.print(p_w);
    Serial.println(" hPa");
    Serial.print("Saettigungs-Dampfdruck 2 " );
    Serial.print(p_w_s);
    Serial.println(" hPa");
    */
  }  
}

bool doPoll = false ;
int pollDelaySeconds ;

#define blen 512

class CmdInterpreter {
  private:
    char buffer[blen+1] ;
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
      sscanf ( msg+3, "%u", &secs ) ;
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
}

unsigned long lastMillis = 0 ;

void newTime () {
  unsigned long now = millis () ;
  unsigned long diff = now - lastMillis ;
  if ( diff == 0 )
    return ;
  unsigned long ncm = clockMillis+diff ;
  if ( ncm > 1000 ) {
    unsigned long incSecs = ncm / 1000L ;
    ncm = ncm - incSecs*1000L ;
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
