#ifndef common
#define common
#include <Arduino.h>
#ifndef defs
  #include "defs.h"
 #endif
#include <EEPROM.h>
#include "GyverTimer.h"
#include <EEPROM.h>


#define Serialmy
#define updint 1000

bool bmpgood=false;

GTimer_ms Tticktime(updint);
GTimer_ms Ttickclock(250);
GTimer_ms Treboot(250000);

#ifdef Lz428266YE

#endif

uint8_t hour;
uint8_t mins;
uint8_t sec;
uint8_t wd;
uint8_t day;
uint8_t month;
int year;
bool syncgood = false;
bool budactiv = false;
volatile int cnt = 0;
bool chimeison=true;

#define budlen 200
#define budton 200
#define budpause 200
#define budcount 5

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
   digitalWrite(pinbeep,LOW);
  #endif
 }
void shortbeep(void){
  beep(125,50);
 }
void longbeep(void){
  beep(250,250);
 }
void dshortbeep(void){
  shortbeep();
  delay(50);
  shortbeep(); 
 } 
void siren(void){

 }
void chimeon(void){chimeison=true;}
void chimeoff(void){chimeison=false;}
void chime(void){
  if (chimeison){longbeep();}
}
void setbud(int n,int ph,int pm){
  EEPROM.write(n*2,ph);
  EEPROM.write(n*2+1,pm);
  EEPROM.commit();
 } 

int getbud(int p){
  return EEPROM.read(p);
 }

void bud(void){
 budactiv=true; 
 for (int i=1;i<=budcount;i++){
  beep(budlen,budton);
  delay(budpause);
  }
  budactiv=false;
 }

bool isbud(){
  if (getbud(0)==0&&getbud(1)==0){return false;}
  return getbud(0)==hour&&getbud(1)==mins&&sec<10;
 }

void ron(void){
  #ifdef relay1pin
   digitalWrite(relay1pin,HIGH);
  #endif
 } 
void roff(void){
  #ifdef relay1pin
   digitalWrite(relay1pin,LOW);
  #endif
 }

void Button(int b){
 switch(b){
  case 0: 
    #ifdef redpin
     digitalWrite(redpin,!digitalRead(redpin));
    #endif
    break;
  case 1: 
    #ifdef bluepin
     digitalWrite(bluepin,!digitalRead(bluepin));
    #endif
    break;
  case 2: 
    #ifdef greenpin
     digitalWrite(greenpin,!digitalRead(greenpin));
    #endif
    break;
  }
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
    digitalWrite(pinbeep,0);
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
  #ifdef relay1pin
   pinMode(relay1pin,OUTPUT);
   #endif   
 }
 #ifndef myBMP280
  float gettemp(void){return -6.28;}
 #endif
#endif
