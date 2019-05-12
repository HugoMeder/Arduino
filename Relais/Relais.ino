void setup() {
  Serial.begin ( 9600 ) ;
  pinMode ( 2, OUTPUT ) ;
}

int state = 0 ;
#define pin 3

void loop() {
  state = (state + 1) & 0x3 ;
  switch ( state ) {
    case 0 : {
      pinMode ( pin, OUTPUT ) ;
      digitalWrite ( pin, HIGH ) ;
      break ;      
    }
    case 1 : {
      pinMode ( pin, INPUT ) ;
      break ;      
    }
    case 2 : {
      pinMode ( pin, OUTPUT ) ;
      digitalWrite ( pin, LOW ) ;
      break ;      
    }
    case 3 : {
      pinMode ( pin, INPUT ) ;
      break ;      
    }
  }
  Serial.println ( state ) ;
  delay ( 1000 ) ;
}
