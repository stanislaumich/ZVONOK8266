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

#undef Serialmy
#define updint 1000

GTimer_ms Tticktime(updint);
GTimer_ms Ttickclock(250);
GTimer_ms Treboot(250000);

uint8_t hour;
uint8_t mins;
uint8_t sec;
uint8_t wd;
uint8_t day;
uint8_t month;
int year;
bool syncgood = false;

volatile int cnt = 0;

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
 }

