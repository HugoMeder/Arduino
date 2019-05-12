#include "DHT.h"

#define DHTPIN 8
#define DHTPIN2 9
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
#define blnkPin 13

bool blnkState = false ;

DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT22 - Test!");

  pinMode ( blnkPin, OUTPUT ) ;
  dht.begin();
  dht2.begin();
}

float saettigungsDampfDruck ( float tempCelsius ) {
    return 6.112*exp ( 17.62*tempCelsius/(243.12+tempCelsius) ) ;
}

float wasserDampfDruck ( float tempCelsius, float relHumidity ) {
    return saettigungsDampfDruck ( tempCelsius )*relHumidity/100.0 ;
}

void loop() {

  delay ( 1000 ) ;
  float h = dht.readHumidity(); //Luftfeuchte auslesen
  float t = dht.readTemperature(); //Temperatur auslesen

  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
  if (isnan(t) || isnan(h))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else
  {
//    Serial.print("Luftfeuchte: ");
    Serial.print("M 1 H ");
    Serial.print(h);
    Serial.print(" %\t");
//    Serial.print("Temperatur: ");
    Serial.print("T ");
//    Serial.print(t);
    Serial.println(t);
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
  if (isnan(t) || isnan(h))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else
  {
//    Serial.print("Luftfeuchte: ");
    Serial.print("M 2 H ");
    Serial.print(h);
    Serial.print(" %\t");
//    Serial.print("Temperatur: ");
    Serial.print("T ");
//    Serial.print(t);
    Serial.println(t);
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
  blnkState = !blnkState ;
  digitalWrite ( blnkPin, blnkState ) ;
}
