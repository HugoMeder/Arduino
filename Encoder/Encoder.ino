
#define DT 2
#define CLK 3

int state = 0 ;
int renum[4] = {0, 1, 3, 2} ;
volatile int cntr = 0 ;
bool clk ;
bool dt ;

void initState () {
  state = dt | (clk << 1);
  state = renum[state] ;
}

void update () {
  int s = dt | (clk << 1);
  s = renum[s] ;
  int diff = (s - state) & 3 ;
  state = s ;
  if ( diff == 1 ) {
    cntr++ ;
    if ( cntr == 41 )
      cntr = 41-80 ;
  } else if ( diff == 3 ) {
    cntr-- ;
    if ( cntr == -40 ) {
      cntr = 80-40 ;
    }
  } else {
    Serial.print ( "!!!!!! diff == " ) ;
    Serial.println ( diff ) ;
  }
  //Serial.print ( "s " ) ;
  //Serial.println ( s ) ;
}

void clkChanged () {
  int clk_ = digitalRead ( CLK ) ;
  if ( clk_ == clk ) {
    /*Serial.print ( "NO CHANGE!!!!!! CLK: " ) ;
      Serial.println ( clk ) ;
    */
  } else {
    clk = clk_ ;
    update () ;
  }
}

void dtChanged () {
  int dt_ = digitalRead ( DT ) ;
  if ( dt_ == dt ) {
    /*Serial.print ( "NO CHANGE!!!!!! DT: " ) ;
      Serial.println ( dt ) ;
    */
  } else {
    dt = dt_ ;
    update () ;
  }
}

void setup() {
  pinMode ( CLK, INPUT ) ;
  pinMode ( DT, INPUT ) ;
  digitalWrite ( CLK, HIGH ) ;
  digitalWrite ( DT, HIGH ) ;
  dt = digitalRead ( DT ) ;
  clk = digitalRead ( CLK ) ;
  initState () ;
  attachInterrupt ( digitalPinToInterrupt(CLK), clkChanged, CHANGE ) ;
  attachInterrupt ( digitalPinToInterrupt(DT), dtChanged, CHANGE ) ;
  //pinMode ( CLK, INPUT ) ;
  Serial.begin ( 9600 ) ;
}

int lastCntr = 0 ;

void loop() {
  if ( lastCntr != cntr ) {
    lastCntr = cntr ;
    Serial.println ( cntr ) ;
  }

}
