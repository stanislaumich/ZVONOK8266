#define common
#include <Arduino.h>
#ifndef place
 #include "place.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif
#include <EEPROM.h>

#define pinbeep 2

void beep(int t1,int t2){
 unsigned long h;
  h=millis();
  while (millis()-h<t1){
   digitalWrite(pinbeep,HIGH);
   delayMicroseconds(t2);
   digitalWrite(pinbeep,LOW);
   delayMicroseconds(t2);
  }
 }

void mLog(String s){
  Serial.println(s);
 }

void initCommon(void){
  Serial.begin(115200);
  EEPROM.begin(512);
 }

