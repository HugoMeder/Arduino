void setup() {
  Serial.begin ( 250000 ) ;
  
  pinMode ( 2, INPUT_PULLUP ) ;//1
  pinMode ( 3, INPUT_PULLUP ) ;//2
  pinMode ( 4, INPUT_PULLUP ) ;//3
  pinMode ( 5, INPUT_PULLUP ) ;//4
  pinMode ( 6, INPUT_PULLUP ) ;//5
  pinMode ( 7, INPUT_PULLUP ) ;//6
  
  //DDRD = 0xFF ;
}

void loop() {
  byte reg = PIND ;
  Serial.println ( reg ) ;
  delay ( 1000 ) ;
}
