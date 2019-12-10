#define Lz428266ZV // Zvonok
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
TickerScheduler ts(2); // количество задач

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
  ts.add(0, 6*60*60*1000, [&](void *) { synctime(); }, nullptr, true);// номер, период, задача, указатель, автозапустить
  ts.add(1, 1000, [&](void *) { showtime(); }, nullptr, true);
  beep(125,50);
  delay(50);
  beep(125,50); 
 }

void loop (){
 ts.update(); // запускаем шедулер   
 goBot();
 httpServer.handleClient();
 }// TOTAL END ///////////////////////////////////////////////////////////////////////