#include <Arduino.h>
#ifndef place
  #include "place.h"
 #endif
#ifndef defs
 #include "defs.h"
 #endif
#ifndef common
 #include "common.h"
 #endif
#ifndef mySSDP
 #include "mySSDP.h"
 #endif
#ifndef myWeb 
 #include "myWebServer.h"
 #endif
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif  
#ifdef Lz428266YE
  //#include "myBMP280.h"  
 #endif 
void dummy(void){}
void showtime(void);
unsigned long prev=0;
void ticktime(){
 /*#ifdef Lz428266ZV
  hour = rtc.now().hour();
  mins = rtc.now().minute();
  sec = rtc.now().second();
  day = rtc.now().date();
  month = rtc.now().month();
  year = rtc.now().year();
  wd = rtc.now().weekday();
 #else*/
  timeClient.update();
  hour=timeClient.getHours();
  mins=timeClient.getMinutes();
  sec=timeClient.getSeconds();
  wd=timeClient.getDay();
  isbud()?bud():dummy();
 //#endif
  showtime();
  if ((sec>=0&&sec<=1)&&mins==0){chime();}
}

void reboot(void){
  longbeep();
  ESP.restart();   
 }

void setup(){  
  initCommon();
  Ttickclock.stop();
  Treboot.stop();
  shortbeep();
  initWiFi();
  initTime();
  #ifdef Lz428266ZV ///////////////////////////////////////////////////////
   //bmpgood =initmyBMP280();
   initLCD1602();
  #endif //////////////////////////////////////////////////////////////////
  #ifdef Lz428266YE ///////////////////////////////////////////////////////
   initmySSD1306();
   bmpgood =initmyBMP280();
  #endif //////////////////////////////////////////////////////////////////
  #ifdef Lz428266WR ///////////////////////////////////////////////////////
   initmyComDisplay();
  #endif //////////////////////////////////////////////////////////////////
  #ifdef Lz428266VFD ///////////////////////////////////////////////////////
   initmyVFD();
  #endif //////////////////////////////////////////////////////////////////   
  initSSDP();
  initWeb();
  MyTeleBotInit();
  bot.sendMessage(myTele, "Бот запущен: "+IP_to_String(WiFi.localIP()), "");
  dshortbeep();
 }

void loop (){
 if (Tticktime.isReady()) ticktime();
 if (Ttickclock.isReady()) tickclock();
 if (Treboot.isReady()) reboot();
 goBot();
 httpServer.handleClient();
 }// TOTAL END ///////////////////////////////////////////////////////////////////////