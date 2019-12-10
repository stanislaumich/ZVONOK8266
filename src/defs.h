#define defs
#define Lz428266ZV
#include "place.h"
#include <ESP8266WiFi.h>
#define myTele "357390016"
//String Esp32Clock="Lz42Esp32Clock";
//String S868 ="519049530";

#ifdef Lz428266
 String myID="947749033";
 String myName="Lz42-8266";
 String Token="";
#endif



#ifdef Lz428266ZV
 String myID="1024759432";
 String myName="Lz428266ZV";
 String myToken="1024759432:AAEqPfDzbySApHVRp5cyqKg2uz6HRGGak9U";
  
#ifdef home
#ifndef STASSID
#define STASSID "ZTE54"
#define STAPSK  "121211119"
#endif
IPAddress ip(192,168,1,238);  
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(194,158,196,206);
IPAddress dns2(194,158,196,205);
#endif
#ifdef work
#ifndef STASSID
#define STASSID "SAN"
#define STAPSK  "37212628"
#endif
IPAddress ip(192,168,0,235);  
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(194,158,196,137);
IPAddress dns2(194,158,196,141);
#endif 

#endif




