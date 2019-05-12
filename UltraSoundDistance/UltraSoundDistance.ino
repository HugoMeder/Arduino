#define Trigger_AusgangsPin 3
#define Echo_EingangsPin 2

void setup() {
  pinMode ( Trigger_AusgangsPin, OUTPUT ) ;
  pinMode ( Echo_EingangsPin, INPUT ) ;
  Serial.begin ( 9600 ) ;
}

int maximumRange = 500; 
int minimumRange = 2; 
double Abstand;
long Dauer;//microseconds
double c = 343.0 ;
double distPerMicrosecond = 0.5*1e-6*c ;

void loop() {
 digitalWrite(Trigger_AusgangsPin, HIGH);
 delayMicroseconds(10); 
 digitalWrite(Trigger_AusgangsPin, LOW);
  
 // Nun wird am Echo-Eingang gewartet, bis das Signal aktiviert wurde
 // und danach die Zeit gemessen, wie lang es aktiviert bleibt 
 Dauer = pulseIn(Echo_EingangsPin, HIGH);
  
 // Nun wird der Abstand mittels der aufgenommenen Zeit berechnet
 Abstand = Dauer*distPerMicrosecond;

  /*
 // Überprüfung ob gemessener Wert innerhalb der zulässingen Entfernung liegt
 if (Abstand >= maximumRange || Abstand <= minimumRange)
 {
    // Falls nicht wird eine Fehlermeldung ausgegeben.
      Serial.println("Abstand außerhalb des Messbereichs");
      Serial.println("-----------------------------------");
 }  
  
 else
 {
    // Der berechnete Abstand wird in der seriellen Ausgabe ausgegeben
      Serial.print("Der Abstand betraegt:");
      Serial.print(Abstand);
      Serial.println("cm");
      Serial.println("-----------------------------------");
 }
 */

       Serial.println(Abstand);

}
