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

