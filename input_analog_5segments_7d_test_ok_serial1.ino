// By Emerson PP6EW
// This code let us to test how good Arduino read the frequency after acquire 
// signals from the counter unit  pb 2086A. Refers to circuit CAT-FT107M
// This program is distributed   WITHOUT ANY WARRANTY and may not be used commercially without the author's permission. 
   
int dial[6][5];
static const uint8_t analog_pins[] = {A4,A3,A2,A1,A0};


#include <EnableInterrupt.h>

// Modify this at your leisure. Refer to https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#Summary

#define PIN4 4  // if 2 for digital input d2
#define PIN5 5
#define PIN6 6   
#define PIN7 7
#define PIN8 8   
#define PIN9 9

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
 
void interruptFunction_d1() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) > 90){
  dial[0][i]=1;
  }else dial[0][i]=0;
}
}
void interruptFunction_d2() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) > 95){  
  dial[1][i]=1;
  }else dial[1][i]=0;
}
}
void interruptFunction_d3() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) >90){  
  dial[2][i]=1;
  }else dial[2][i]=0;
}
}
void interruptFunction_d4() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) > 90){
  dial[3][i]=1;
  }else dial[3][i]=0;
}
}
void interruptFunction_d5() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) > 90){
  dial[4][i]=1;
  }else dial[4][i]=0;
}
}
void interruptFunction_d6() {
  for (int i = 0; i < 5; i++) { // a-g
    if ( (analogRead(analog_pins[i])) > 92){
  dial[5][i]=1;
  }else dial[5][i]=0;
}
}
void setup() {
  analogReference(INTERNAL);  // AREF  1.1V
sbi(ADCSRA,ADPS2) ;
cbi(ADCSRA,ADPS1) ;
cbi(ADCSRA,ADPS0) ;
DDRC = B00000000; // set all analog pins as input

  for (int i = 0; i < 5; i++) {
    dial[0][i]=0;
  }
  
 pinMode(PIN4, INPUT);   
 pinMode(PIN5, INPUT); 
 pinMode(PIN6, INPUT);   
 pinMode(PIN7, INPUT); 
 pinMode(PIN8, INPUT);  
 pinMode(PIN9, INPUT_PULLUP); 
enableInterrupt(PIN4 | PINCHANGEINTERRUPT, interruptFunction_d1, FALLING);
enableInterrupt(PIN5 | PINCHANGEINTERRUPT, interruptFunction_d2, FALLING);
enableInterrupt(PIN6 | PINCHANGEINTERRUPT, interruptFunction_d3, FALLING);
enableInterrupt(PIN7 | PINCHANGEINTERRUPT, interruptFunction_d4, FALLING);
enableInterrupt(PIN8 | PINCHANGEINTERRUPT, interruptFunction_d5, FALLING);
enableInterrupt(PIN9 | PINCHANGEINTERRUPT, interruptFunction_d6, FALLING);
    
 Serial.begin(38400,SERIAL_8N1);
  
}

void loop() { 
  
   if (dial[5][0]*dial[5][0]*dial[5][0]*dial[5][0]*dial[5][0]*dial[5][0]!=0){
   Serial.print(display_number(dial[5]), DEC);
 Serial.print(display_number(dial[4]), DEC);
 Serial.print(".");
 Serial.print(display_number(dial[3]), DEC);
 Serial.print(display_number(dial[2]), DEC);
 Serial.print(display_number(dial[1]), DEC);
 Serial.print(".");
 Serial.println(display_number(dial[0]), DEC);
   }else{
    Serial.print(display_number(dial[4]), DEC);
 Serial.print(".");
 Serial.print(display_number(dial[3]), DEC);
 Serial.print(display_number(dial[2]), DEC);
 Serial.print(display_number(dial[1]), DEC);
 Serial.print(".");
 Serial.println(display_number(dial[0]), DEC);
    
    }
  
 delay(1999);
// Serial.print('b');
 // Serial.print('d');
 //  Serial.print('e');
 // Serial.print('f');
 // Serial.println('g');
 //  Serial.print(dial[5][0], DEC);
 // for (int j=1; j < 4;j++){
 // Serial.print(dial[5][j], DEC);}
//  Serial.println(dial[5][4], DEC); 

  
}

// function input b,d,e,f,g and return number
int display_number(int d[]){ // return the digit numbers
int nu;
if (d[0]*d[1]*d[2]*d[3]*d[4] != 0 ){ nu=8;
}else if (d[0]==0){
if (d[2]==0) {  nu=5;
} else {nu= 6;
}
} else if (d[0]*d[1]*d[2]!=0){
if(d[3]==0){nu=2;
} else{nu=0;
}
} else if (d[4]==0){
if(d[3]==0){nu=1;
} else{nu=7;
}
} else if (d[3]*d[4]!=0){
  if(d[1]==0){ nu=4;
  } else{nu=9;
  }
} else{ nu= 3;
}                                      
return (nu);
}
