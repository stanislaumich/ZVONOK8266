#ifndef common
#define common
#ifndef SettingsOled
 #include "SettingsOled.h"
#endif
#include <Preferences.h>

String Esp32Clock="Lz42Esp32Clock";
String Lz428266="Lz42-8266";
String S868 ="519049530";
String myID="852912728";
String myName="Lz42Esp32Clock";

Preferences prefs;
int arrBut[3]={0,0,0};

#define pinp 14
#define pinn 15

void beep(int t1,int t2){
 unsigned long h;
  if(myON){
  h=millis();
  while (millis()-h<t1){
   digitalWrite(pinp,HIGH);
   delayMicroseconds(t2);
   digitalWrite(pinp,LOW);
   delayMicroseconds(t2);
  }
  }
 }

void Button_init(void){
  pinMode(pinp,OUTPUT);
  pinMode(pinn,OUTPUT);
  digitalWrite(pinp,LOW);
  digitalWrite(pinn,LOW);
  prefs.begin("button");
  int z =prefs.getBytes("button",arrBut,sizeof(arrBut)); 
 }
void Button(int state){
  arrBut[state]?arrBut[state]=0:arrBut[state]=1;
  addds("State "+String(arrBut[state])+" switched");
  prefs.putBytes("buttons",arrBut,sizeof(arrBut));
 }

String getValue(String data, char separator, int index){
  // String part01 = getValue(application_command,';',0); 
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
 }  



int getButton(int b){
  return arrBut[b];
 }


int getBud(int p){
  if (p==0){   
   return prefs.getInt("alarm_h", 0);
  }
  if (p==1){
   return prefs.getInt("alarm_m",0);
  }
 }

void setBud(int ph,int pm){
  size_t q1 = prefs.putInt("alarm_h", ph);
  size_t q2 = prefs.putInt("alarm_m", pm);
 } 

#endif  