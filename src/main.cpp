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
TickerScheduler ts(2); // количество задач

void setup(){
  initCommon();
  #ifdef Lz428266ZV ///////////////////////////////////////////////////////
   initRTC();
   initLCD1602();
  #endif //////////////////////////////////////////////////////////////////0
  initWiFi();
  initSSDP();
  initWeb();

  ts.add(0, 6*60*60*1000, [&](void *) { synctime(); }, nullptr, true);// номер, период, задача, указатель, автозапустить
  ts.add(1, 1000, [&](void *) { showtime(); }, nullptr, true);
 }

void loop (){
 ts.update(); // запускаем шедулер   

 }// TOTAL END ///////////////////////////////////////////////////////////////////////