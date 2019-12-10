#define Lz428266YE // Zvonok
#include <Arduino.h>
#include <TickerScheduler.h>
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
#ifndef nodisplay
 #include "nodisplay.h"
 #endif
#endif //Lz428266YE

#ifndef mySSDP
 #include "mySSDP.h"
 #endif
#ifndef myWeb 
#include "myWebServer.h"
#endif
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif  
TickerScheduler ts(3); // количество задач

void showtime(void);

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
  sec = sec + 1;
  mins = mins + 1;
  if(mins==60){
    mins = 0;
    hour = hour + 1;
  }
  if (hour==24){
    hour = 0;
  }
  if (hour%2==0){
   syncgood = GetNTP();
   if (syncgood){
    mins = ( ntp_time / 60 ) % 60;
    hour = ( ntp_time / 3600 ) % 24;
    sec=ntp_time % 60;
    wd=(ntp_time/60/60/24+4)%7;  
   }
  }
 #endif
 showtime();
}

void setup(){
  beep(125,50);
  initCommon();
  #ifdef Lz428266ZV ///////////////////////////////////////////////////////
   initRTC();
   initLCD1602();
  #endif //////////////////////////////////////////////////////////////////0
  initWiFi();
  initSSDP();
  initWeb();
  MyTeleBotInit();
  bot.sendMessage(myTele, "Бот запущен: "+IP_to_String(WiFi.localIP()), "");
  ts.add(0, 6*60*60*1000, [&](void *) { ticktime(); }, nullptr, true);// номер, период, задача, указатель, автозапустить
  ts.add(1, 1000, [&](void *) { ticktime(); }, nullptr, true);
  ts.add(2, 250, [&](void *) { tickclock(); }, nullptr, false);
  beep(125,50);
  delay(50);
  beep(125,50); 
 }

void loop (){
 ts.update(); // запускаем шедулер   
 goBot();
 httpServer.handleClient();
 }// TOTAL END ///////////////////////////////////////////////////////////////////////