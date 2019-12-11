#ifndef mySSD1306
#define mySSD1306
#include "SSD1306.h"
#include "font_orbitum_20.h"
#ifndef defs
 #include "defs.h"
 #endif
#ifndef common
 #include "common.h"
 #endif 
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif
#ifndef myTime
  #include "myTime.h"
  #endif
#define OLED_SDA    5  // pin 14 gpio5!
#define OLED_SDC    4  // pin 12 gpio4!
#define OLED_ADDR   0x3C

SSD1306  display(OLED_ADDR, OLED_SDA, OLED_SDC);
bool ssd1306ON = true;

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
   display.setFont(Orbitron_Medium_20);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   String tempstr=timeClient.getHours()+":"+timeClient.getMinutes();
   display.drawString(2, 4, tempstr);
   //display.drawString(2, 4,"12345678");
   display.display();
}

void initmySSD1306(void){
  display.init();
  screenon();
  //display.flipScreenVertically();
   display.setFont(Orbitron_Medium_20);
   display.setTextAlignment(TEXT_ALIGN_CENTER);
   
   display.clear();
   display.setFont(Orbitron_Medium_20);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(1, 4, "Start");
   display.display();
   delay(2000);
 }
#endif