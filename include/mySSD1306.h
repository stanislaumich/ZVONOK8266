#ifndef mySSD1306
#define mySSD1306
#include "SSD1306.h"
#include "font_orbitum_32.h"
#define fnt Orbitron_Medium_32 //24 w secs
#ifndef place
  #include "place.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif
#ifndef common
 #include "common.h"
 #endif 
//#ifndef MyTeleBot
//  #include "myTeleBot.h"
// #endif
//#ifndef myTime
//  #include "myTime.h"
//  #endif
#define OLED_SDA    5  // pin 14 gpio5!
#define OLED_SDC    4  // pin 12 gpio4!
#define OLED_ADDR   0x3C

SSD1306  display(OLED_ADDR, OLED_SDA, OLED_SDC);
bool ssd1306ON = true;
String ssd13str="";
void clrscr(void){
 display.clear();
 } 
void screenoff(void){
  display.displayOff();
  ssd1306ON=false;
 }
void screenon(void){
  display.displayOn();
  ssd1306ON=true;
 }

void tickclock(void){
  
 }

void showtime(void){
   display.clear();
   display.setFont(fnt);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   //ssd13str=timeClient.getHours()+":"+timeClient.getMinutes();
   //hour = timeClient.getHours();
   //mins = timeClient.getMinutes();
   //sec =  timeClient.getSeconds();
   //ssd13str = random(100000);
   ssd13str = (hour<10?"0"+String(hour):String(hour))+String(":")+(mins<10?"0"+String(mins):String(mins));
   //ssd13str = (hour<10?"0"+String(hour):String(hour))+String(":")+(mins<10?"0"+String(mins):String(mins))+String(":")+(sec<10?"0"+String(sec):String(sec));
   display.drawString(6, 4, ssd13str);
   //display.drawString(2, 4,"12345678");
   display.display();
}

void initmySSD1306(void){
  display.init();
  screenon();
  //display.flipScreenVertically();
   display.clear();
   display.setFont(fnt);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(1, 4, "Start");
   display.display();
   delay(2000);
 }
#endif