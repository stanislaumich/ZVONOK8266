#ifndef myTime
#define myTime
#ifndef defs
  #include "defs.h"
 #endif 
#ifndef common
 #include "common.h"
#endif 
#ifndef myWiFi
 #include "myWiFi.h"
#endif
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif
#include <NTPClient.h> 
#include <WiFiUdp.h>
//#include <EEPROM.h>

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600*3, 60000); 
unsigned long ntp_time = 0;

String millis2time(){
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
 }


String getTimestr(void){
 return timeClient.getFormattedTime(); 
 }
void initTime(void){
     timeClient.begin();
 }
#endif
