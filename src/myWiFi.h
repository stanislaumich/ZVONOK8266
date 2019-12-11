#define myWiFi
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#ifndef defs
 #include "defs.h"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

void initWiFi(void){
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(myName);
  WiFi.config(ip, gateway, subnet, dns1, dns2);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    //Serial.println("WiFi failed, retrying...");
  }
 }

String IP_to_String(IPAddress ip){
  return(String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]));
 }

IPAddress String_to_IP(String strIP){
 int Parts[4] = {0,0,0,0};
 int Part = 0;
 for ( byte i=0; i<strIP.length(); i++ )
 {
  char c = strIP.charAt(i);
  if ( c == '.' )
  {
    Part++;
    continue;
  }
  if ( c<48 || c>57 ) continue; // не цифровой символ   
  Parts[Part] *= 10;
  Parts[Part] += c - '0';
 }
 IPAddress ip_str( Parts[0], Parts[1], Parts[2], Parts[3] );
 return(ip_str);
 }