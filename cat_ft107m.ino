/*************************************************************************
 * FT107M CAT Library, by Emerson PP6EW
 * 
 * This code create a CAT for yaesu FT107M radio.
 *  
 *
 * The original code has been built by Pavel Milanes, CO7WT, pavelmc@gmail.com
 *  with the review of various sources:
 * - James Buck, VE3BUX, FT857D arduino Lib [http://www.ve3bux.com]
 * - Hamlib source code
 * - FLRig source code
 * - Chirp source code
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * **************************************************************************/


#include <ft857d.h>
#include <EnableInterrupt.h>

ft857d radio = ft857d();

// variables
long  freq = 0;
boolean ptt = false;
boolean splitActive = false;
boolean vfoAActive = true;
byte mode = 0;
int dial[6][5];
static const uint8_t analog_pins[] = {A4,A3,A2,A1,A0};

// radio modes
#define MODE_LSB 00
#define MODE_USB 01
#define MODE_CW 02

#define switch_ptt 10
#define freq_up 11
#define freq_down 13
#define f_fast 12
// number of analog samples to take per reading
#define NUM_SAMPLES 3
#define PIN4 4  // if 2 sig d2  ou A0
#define PIN5 5
#define PIN6 6  // if 2 sig d2  ou A0
#define PIN7 7
#define PIN8 8  // if 2 sig d2  ou A0
#define PIN9 9

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void interruptFunction_d1() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) > 90){  
  dial[0][i]=1;
  }else dial[0][i]=0;
}
}
void interruptFunction_d2() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) > 95){  
  dial[1][i]=1;
  }else dial[1][i]=0;
}
}
void interruptFunction_d3() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) >90){ 
  dial[2][i]=1;
  }else dial[2][i]=0;
}
}
void interruptFunction_d4() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) > 90){ 
  dial[3][i]=1;
  }else dial[3][i]=0;
}
}
void interruptFunction_d5() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) > 90){  
  dial[4][i]=1;
  }else dial[4][i]=0;
}
}
void interruptFunction_d6() {
  for (int i = 0; i < 5; i++) { // bdefg
    if ( (analogRead(analog_pins[i])) > 92){  
  dial[5][i]=1;
  }else dial[5][i]=0;
}
}
// function input a,b,c,d,e,f,g and return number
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
// DEBUG flag, uncomment it if you want to test it by hand
//#define DEBUG true

// function to run when we must put radio on TX/RX
void catGoPtt(boolean pttf) {
    // the var ptt follows the value passed, but you can do a few more thing here
    ptt = pttf;
    if (ptt == true) {
      digitalWrite(switch_ptt, HIGH);
    }else {
      digitalWrite(switch_ptt, LOW);
      }

    #if defined (DEBUG)
    // debug
    Serial.print("PTT Status is: ");
    Serial.println(ptt);
    #endif
}

// function to run when VFOs A/B are toggled
void catGoToggleVFOs() {
    // here we simply toggle the value
    vfoAActive = !vfoAActive;

    #if defined (DEBUG)
    // debug
    Serial.print("VFO A active?: ");
    Serial.println(vfoAActive);
    #endif
}

// function to set a freq from CAT
void catSetFreq(long f) {
  int di[4];
  int xx,status;
  long del_f;
  for (int i = 0; i < 4; i++) di[i]=0;
   
  // if (f < 7500000 & f > 7000000){ // test on 40m
     if ( f > freq){
  del_f=f-freq;
  xx = 11;  
  } else{
  del_f=freq-f;
  xx = 13;
  } 
        
    del_f=del_f/100;
    int kl=0;
    while (del_f!=0){
      di[kl] = del_f%10;
      kl+=1;
      if (del_f%10!=0){
        del_f=del_f- del_f%10 ;
      }
     del_f=del_f/10;   
    }
  for (int j=0; j < 4; j++){
      
    
   for (int i = 0; i < di[j]; i++){
          switch (j){
      case 0:
      digitalWrite(xx, HIGH);
      delay(150);
      digitalWrite(xx, LOW);
      break;
       case 1:
      digitalWrite(xx, HIGH);
      delayMicroseconds(100);
      digitalWrite(f_fast, HIGH);
      delay(83);
      digitalWrite(f_fast, LOW);
      delayMicroseconds(100);
      digitalWrite(xx, LOW);
      break;
      case 2:
      digitalWrite(xx, HIGH);
       delayMicroseconds(100);
      digitalWrite(f_fast, HIGH);
      delay(836);
      digitalWrite(f_fast, LOW);
       delayMicroseconds(100);
      digitalWrite(xx, LOW);
      break;
      case 3:
      digitalWrite(xx, HIGH);
      delayMicroseconds(100);
      digitalWrite(f_fast, HIGH);
      delay(8362);
      digitalWrite(f_fast, LOW);
      delayMicroseconds(100);
      digitalWrite(xx, LOW);
      break;
  }
          }
  
  }
 
 // }
   
    
    // the var freq follows the value passed, but you can do a few more thing here
    

    #if defined (DEBUG)
    // debug
    Serial.print("Active VFO freq is: ");
    Serial.println(freq);
   
    #endif
    
}

// function to set the mode from the cat command
void catSetMode(byte m) {
    // the var mode follows the value passed, but you can do a few more thing here
    mode = m;

    #if defined (DEBUG)
    // debug
    Serial.print("Active VFO mode is: ");
    Serial.println(mode);
    #endif
}

// function to pass the freq to the cat library
long catGetFreq() {
  long f0=(long)display_number(dial[0])*100;
  long f1=(long)display_number(dial[1])*1000;
  long f2=(long)display_number(dial[2])*10000;
  long f3=(long)display_number(dial[3])*100000;
  long f4=(long)display_number(dial[4])*1000000;
  long f5=(long)display_number(dial[5])*10000000;
  
    // this must return the freq as an unsigned long in Hz, you must prepare it before
    if (dial[5][0]==0 && dial[5][1]==0 && dial[5][4]==0){
      freq=f0+f1+f2+f3+f4; 
      } else {
        freq=f0+f1+f2+f3+f4+f5; 
        }
    
   // for (int j=0;j < 6; j++ ){
    //  freq=display_number(dial[j])*1000
    //  }     


    #if defined (DEBUG)
    // debug
    Serial.println("Asked for freq");
    #endif

    // pass it away
    return freq;
}

// function to pass the mode to the cat library
byte catGetMode() {
    // this must return the mode in the wat the CAT protocol expect it

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for mode");
    #endif

    // pass it away
    return mode;
}

// function to pass the smeter reading in RX mode
byte catGetSMeter() {
    // this must return a byte in with the 4 LSB are the S meter data
    // so this procedure must take care of convert your S meter and scale it
    // up to just 4 bits
    byte smet = 8;// start value
    #if defined (DEBUG)
    //debug
    Serial.println("Asked for S meter");
    #endif
 
   
    int sum = 0;                    // sum of samples taken
    unsigned char sample_count = 0; // current sample number
    float voltage = 0.0;            // calculated voltage

    // take a number of analog samples and add them up
   while (sample_count < NUM_SAMPLES) {
      sum += analogRead(A7);
        sample_count++;
   }
    // calculate the voltage
    // use 0.231 ADC reference voltage 
    //  ??? V is the calibrated reference voltage
    voltage = ((float)sum / (float)NUM_SAMPLES * 1.1) / 1023.0;
     
    //Serial.print(voltage);
    //Serial.println (" V");
    sample_count = 0;
    sum = 0;
    smet = byte(63 *voltage +2); // converte 0-AREF=1.1 to 0-15 
    return (smet);
 
  return (smet);
}

// function to pass the TX status
byte catGetTXStatus() {
    /*
     * this must return a byte in wich the different bits means this:
     * 0b abcdefgh
     *  a = 0 = PTT on
     *  a = 1 = PTT off
     *  b = 0 = HI SWR off
     *  b = 1 = HI SWR on
     *  c = 0 = split on
     *  c = 1 = split off
     *  d = dummy data
     *  efgh = PO meter data
     */

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for TX status");
    #endif

    // you have to craft the byte from your data, we will built it from
    // our data
    byte r = 1;
    // we fix the TX power to half scale (8)
    r = ptt<<7 + splitActive<<5 + 8;

    return r;
}


void setup() {
  analogReference(INTERNAL);  // AREF
  sbi(ADCSRA,ADPS2) ; //s
cbi(ADCSRA,ADPS1) ;   //c
cbi(ADCSRA,ADPS0) ;   //c
DDRC = B00000000; // set all analog pins as input
  pinMode(switch_ptt, OUTPUT);
  pinMode(freq_up, OUTPUT);
  pinMode(freq_down, OUTPUT);
  pinMode(f_fast, OUTPUT);
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
  
  delay(50);
    // preload the vars in the cat library
    radio.addCATPtt(catGoPtt);
    radio.addCATAB(catGoToggleVFOs);
    radio.addCATFSet(catSetFreq);
    radio.addCATMSet(catSetMode);
    radio.addCATGetFreq(catGetFreq);
    radio.addCATGetMode(catGetMode);
    radio.addCATSMeter(catGetSMeter);
    radio.addCATTXStatus(catGetTXStatus);

    // now we activate the library
    radio.begin(38400, SERIAL_8N1);

    #if defined (DEBUG)
    // serial welcome
    Serial.println("CAT Serial Test Ready");
    #endif

}

void loop() {
    radio.check();
}
