void setup() {
  pinMode ( 13, OUTPUT ) ;
}

bool state = false ; 

void loop() {
  state = ! state ;
  digitalWrite ( 13, state );
  delay ( 2000 ) ;  
  // put your main code here, to run repeatedly:

}
