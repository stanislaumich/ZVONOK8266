//#define Lz428266ZV // Zvonok
#define Lz428266YE // YELLOW
//#define Lz428266VFD // VFD
//#define Lz428266WR // work, uno+8266
#include <Arduino.h>

#ifndef common
 #include "common.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif


#ifdef Lz428266ZV 
 #ifndef myRTC
  #include "myRTC.h"
  #endif
 #ifndef lcd1602
  #include "lcd1602.h"
  #endif
 #endif //Lz428266ZV
#ifdef Lz428266YE 
 #ifndef myTime
  #include "myTime.h"
  #endif
 #ifndef mySSD1306
  #include "mySSD1306.h"
  #endif
 #endif //Lz428266YE  
#ifdef Lz428266WR 
 #ifndef myTime
  #include "myTime.h"
  #endif
 #ifndef nodisplay
  #include "nodisplay.h"
  #endif 
 #endif //Lz428266WR
#ifdef Lz428266VFD 
 #ifndef myTime
  #include "myTime.h"
  #endif
 #ifndef myVFD
  #include "myVFD.h"
  #endif
 #endif //Lz428266VFD

#ifndef mySSDP
 #include "mySSDP.h"
 #endif
#ifndef myWeb 
 #include "myWebServer.h"
 #endif
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif  
//TickerScheduler ts(3); // количество задач

void showtime(void);
unsigned long prev=0;
void ticktime(){
 #ifdef Lz428266ZV
  hour = rtc.now().hour();
  mins = rtc.now().minute();
  sec = rtc.now().second();
  day = rtc.now().date();
  month = rtc.now().month();
  year = rtc.now().year();
 #endif
 #ifdef Lz428266YE
   timeClient.update();
 #endif
 #ifdef Lz428266VFD
   timeClient.update();
 #endif
 #ifdef Lz428266WR
  
 #endif 
 //showtime();
   display.clear();
   display.setFont(Orbitron_Medium_20);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   ssd13str=timeClient.getHours()+":"+timeClient.getMinutes();
   display.drawString(2, 4, ssd13str);
   //display.drawString(2, 4,"12345678");
   display.display();
}

void reboot(void){
  ESP.restart();   
 }

void setup(){  
  initCommon();
  #ifdef Lz428266ZV ///////////////////////////////////////////////////////
   initRTC();
   initLCD1602();
  #endif //////////////////////////////////////////////////////////////////0
  #ifdef Lz428266YE ///////////////////////////////////////////////////////
   initmySSD1306();
  #endif //////////////////////////////////////////////////////////////////0
  beep(125,50);
  //Ttickclock.stop();
  //Treboot.stop();
  initWiFi();
  initTime();
  initSSDP();
  initWeb();
  MyTeleBotInit();
  bot.sendMessage(myTele, "Бот запущен: "+IP_to_String(WiFi.localIP()), "");
  beep(125,50);
  delay(50);
  beep(125,50); 
 }

void loop (){
 //if (Tticktime.isReady()) ticktime();
 //if (Ttickclock.isReady()) tickclock();
 //if (Treboot.isReady()) reboot();
 goBot();
 httpServer.handleClient();
 unsigned long c=millis();
 if (c-prev>1000){
  timeClient.update();
  showtime();
 }
 }// TOTAL END ///////////////////////////////////////////////////////////////////////