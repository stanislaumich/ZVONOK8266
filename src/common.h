#define common
#include <Arduino.h>
#ifndef place
 #include "place.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif
#include <EEPROM.h>
#include "GyverTimer.h"
/*
place
#define work
//#define Lz428266ZV // Zvonok
//#define Lz428266YE // YELLOW
//#define Lz428266VFD // VFD
#define Lz428266WR // work, uno+8266
*/
#undef Serialmy
#define updint 1000

GTimer_ms Tticktime(updint);
GTimer_ms Ttickclock(250);
GTimer_ms Treboot(250000);
// 15 RED, 14 - рядом маленький 16 - рядом ещё ближе  13 - синий 12 зелёный
#ifdef Lz428266YE
 #define redpin 15
 #define bluepin 13
 #define greenpin 12
#endif

uint8_t hour;
uint8_t mins;
uint8_t sec;
uint8_t wd;
uint8_t day;
uint8_t month;
int year;
bool syncgood = false;

volatile int cnt = 0;

void Button(int b){
 int state;
 switch(b){
  case 0: 
    #ifdef redpin
     digitalRead(redpin, state);
     digitalWrite(redpin,!state);
    #endif
    break;
  case 1: 
    #ifdef bluepin
     digitalRead(bluepin, state);
     digitalWrite(bluepin,!state);
    #endif
    break;
  case 2: 
    #ifdef greenpin
     digitalRead(greenpin, state);
     digitalWrite(greenpin,!state);
    #endif
    break;
 }

 }

void beep(int t1,int t2){
  #ifdef pinbeep
  unsigned long h;
  h=millis();
  while (millis()-h<(unsigned int)(t1)){
   digitalWrite(pinbeep,HIGH);
   delayMicroseconds(t2);
   digitalWrite(pinbeep,LOW);
   delayMicroseconds(t2);
  }
  #endif
 }

void mLog(String s){
  #ifdef Serialmy
   Serial.println(s);
  #endif 
 }

void initCommon(void){
   #ifdef Serialmy
    Serial.begin(115200); 
   #endif
  EEPROM.begin(512);
  #ifdef pinbeep
    pinMode(pinbeep,OUTPUT);
   #endif
  #ifdef redpin
    pinMode(redpin,OUTPUT);
   #endif
  #ifdef bluepin
    pinMode(bluepin,OUTPUT);
   #endif
  #ifdef greenpin
    pinMode(greenpin,OUTPUT);
   #endif   
 }

