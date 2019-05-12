#define pin 13

void setup() {
  pinMode ( pin, OUTPUT ) ;
}

bool state = true ; 

void loop() {
  state = ! state ;
  digitalWrite ( pin, state );
}
