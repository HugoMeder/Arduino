#include "Arduino.h"

void setup() {
  Serial.begin ( 250000 ) ;

}

double relHum[8] = {
  20,
  30,
  40,
  50,
  60,
  70,
  80,
  90
} ;

double temp[9] = {
  10,
  15,
  20,
  25,
  30,
  35,
  40,
  45,
  50
} ;

double table[9][8] = {
  {
    9000,
    2500,
    740,
    220,
    72.00,
    25.80,
    9.50,
    3.72
  },
  {
    6364,
    1803,
    543,
    166,
    55.64,
    20.94,
    8.07,
    3.26
  },
  {
    4500,
    1300,
    398,
    125,
    43.00,
    17.00,
    6.85,
    2.85
  },
  {
    2890,
    900,
    270,
    81,
    33.00,
    13.00,
    5.30,
    2.20,
  },
  {
    2100,
    670,
    210,
    66,
    25.50,
    10.20,
    4.28,
    1.85,
  },
  {
    1652,
    530,
    168,
    54,
    21.54,
    8.69,
    3.71,
    1.62,
  },
  {
    1300,
    420,
    135,
    45,
    18.20,
    7.40,
    3.22,
    1.41
  },
  {
    1020,
    317,
    103,
    35,
    14.28,
    6.02,
    2.67,
    1.20
  },
  {
    800,
    240,
    79,
    27,
    11.20,
    4.90,
    2.22,
    1.02
  }
};

int index[1] ;
double w[2] ;

void getIndexAndWeights( double temp, int* index, double* w) {
  if ( temp <= 10.0 || temp >= 50.0 ) {
    index[0] = -1 ;
    return ;
  }
  double dind = (temp - 10.0) / 5.0 ;
  int i = (int)floor ( dind ) ;
  index[0] = i ;
  w[1] = dind - i ;
  w[0] = 1.0 - w[1] ;
}

double getRelHum ( double temp, double resist ) {
  resist /= 1000.0 ;
  getIndexAndWeights ( temp, index, w ) ;
  int i = index[0] ;
  if ( i == -1 )
    return -200.0 ;
  double* r0 = table[i] ;
  double* r1 = table[i + 1] ;
  double w0 = w[0] ;
  double w1 = w[1] ;
  double r_0 = r0[0] * w0 + r1[0] * w1 ;
  if ( resist > r_0 ) {
    return -100.0 ;
    // throw new Error ( "resist > r_0" ) ;
  }
  int rind = 1 ;
  double r_1 = r0[rind] * w0 + r1[rind] * w1 ;
  while ( r_1 > resist ) {
    rind++ ;
    if ( rind > 7 )
      return -100 ;
    r_0 = r_1 ;
    r_1 = r0[rind] * w0 + r1[rind] * w1 ;
  }
  w0 = (resist - r_1) / (r_0 - r_1) ;
  w1 = 1.0 - w0 ;
  return relHum[rind-1] * w0 + relHum[rind] * w1 ;
}




double humidity ( double temp ) {
  int val = analogRead ( 7 ) ;
  double resist = 145000.0 * val / (1024.0 - val) ;
  Serial.print ( "R " ) ;
  Serial.print ( resist ) ;
  Serial.println ( " ohm" ) ;
  return getRelHum ( temp, resist ) ;
}

double getTempOhm () {
  int val = analogRead ( 6 ) ;
  // Serial.print ( "temp val " ) ;
  // Serial.println ( val ) ;
  double rv = 1000.0 * val / (1024.0 - val) ; 
  return rv ;
}

double temperature () {
  double tempOhm = getTempOhm () ;
  //  Serial.print ( tempOhm ) ;
  //  Serial.println ( " ohm" ) ;
  return (tempOhm / 1000.0 - 1.0) / 3.851e-3 ;
}
void temp_ () {
  double T = temperature () ;
  Serial.print ( "temp " ) ;
  Serial.print ( T ) ;
  Serial.println ( " degree" ) ;
}
void loop() {
  double temp = temperature () ;
  double relHum = humidity ( temp ) ;
  Serial.print ( "temp " ) ;
  Serial.print ( temp ) ;
  Serial.print ( " degree, " ) ;
  Serial.print ( "humidity " ) ;
  Serial.print ( relHum ) ;
  Serial.println ( " %" ) ;
  delay ( 1000 ) ;
}
