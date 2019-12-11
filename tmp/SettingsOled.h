// http://oleddisplay.squix.ch/#/home
#define SettingsOled
#include "SSD1306.h"
// #include "font_orbitum_10.h"  // очень красиво
#include "font_orbitum_18.h"
#include "font_orbitum_20.h"
#include "font_orbitum_10.h"
#define FONTBIG Orbitron_Medium_20
#define FONTSMALL Orbitron_Medium_10
#ifndef common
 #include "common.h"
#endif
/*#ifndef MyTime
 #include "Mytime.h"
#endif*/  
#define OLED_SDA    5  // pin 14
#define OLED_SDC    4  // pin 12
#define OLED_ADDR   0x3C

#define font10

#ifdef font10
#define sfy 1
#define s0y 18
#define s1y 30
#define s2y 42
#define s3y 54
#endif

#ifdef font12
#define sfy 1
#define s0y 18
#define s1y 30
#define s2y 42
#define s3y 54
#endif

SSD1306  display(0x3c, 5, 4);
String ds[4];
String mIP;
String fStr1;
String timestr;
String fStr;
bool myON;
unsigned long screentimeoutr=3000;
unsigned long screentimeout=3000;
unsigned long screencur;

void screenoff(void){
  display.displayOff();
 }
void screenon(void){
  display.displayOn();
 } 

void initOled(void){
  display.init();
  //display.flipScreenVertically();
  //display.setFont(ArialMT_Plain_10);
  //display.setFont(FONTBIG);
  myON=true;
  }
String formfirst(void){
  //return fStr1+(String)" "+timestr;
  return timestr; 
}

void wrds(int type){
   if (myON) {screenon();}
   display.clear();
   //fStr=formfirst();
   if (type==1){
   display.setFont(FONTSMALL);
   display.setTextAlignment(TEXT_ALIGN_LEFT);  
   display.drawString(1, sfy, formfirst());
   display.drawString(1, s0y, ds[0]);
   display.drawString(1, s1y, ds[1]);
   display.drawString(1, s2y, ds[2]);
   display.drawString(1, s3y, ds[3]);

   }
   if (type==0){
    display.setFont(FONTBIG);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 23, formfirst());
   }

   display.display();
   screencur=millis(); 
  }
/*
void wrds(void){
   if (myON) {screenon();}
   display.clear();
   //fStr=formfirst();
   display.setFont(Orbitron_Medium_18);
   display.setTextAlignment(TEXT_ALIGN_CENTER);

   display.drawString(64, 20, formfirst());
   /*display.drawString(1, s0y, ds[0]);
   display.drawString(1, s1y, ds[1]);
   display.drawString(1, s2y, ds[2]);
   display.drawString(1, s3y, ds[3]);
   display.display();
   screencur=millis(); 
  }
*/
void addds(String qq){
  ds[0]=ds[1];
  ds[1]=ds[2];
  ds[2]=ds[3];
  ds[3]=qq;
  wrds(1);
 }
void insds(int pos,String qq){
  ds[pos]=qq;
  wrds(1);
 } 
void clrscr(void){
 display.clear();
 } 
   