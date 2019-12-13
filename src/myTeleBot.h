#ifndef MyTeleBot
#define MyTeleBot
#ifndef common
  #include "common.h"
 #endif
#ifndef defs
  #include "defs.h"
 #endif 
//#ifdef Lz428266WR
 #ifndef myTime
   #include "myTime.h"
  #endif
// #endif
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

BearSSL::WiFiClientSecure client;
UniversalTelegramBot bot(myToken, client); 
 
int Bot_mtbs = 1000; 
long Bot_lasttime;   
bool Start = false;

const int ledPin = 13;
int ledStatus = 0; 

volatile bool debug=0;
String from_name="";
String execCommand(String chat_id, String text);
String getTimestr(void);

String sendtobot(String ch_id, String mess){
  String m="="+ch_id+"="+myName+"="+mess;
  ch_id==LzName?bot.sendMessage(LzName, m, ""):bot.sendMessage(S868, m, "");
  return m;
}

String millis2time();

void answerbot(String chat_id, String text){  
  String mess="-? > "+text;
  if(text.indexOf("=")==0){
    text.remove(0,1);
    int p = text.indexOf('=');
    String name = text.substring(0, p);
    text.remove(0,p+1);
    p = text.indexOf('=');
    name = text.substring(0,p);
    text.remove(0,p+1);
    text.trim();
    sendtobot(name, execCommand(chat_id,text));
   }
   else{
    bot.sendMessage(chat_id,execCommand(chat_id,text));
   }  
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

String execCommand(String chat_id, String text){
   String answ="? - > "+text;
   if (text == "/b0") {
       Button(0);
      answ="+Ok /b0";
    }
    
   if (text == "/d") {
      debug=!debug;
      debug?answ="+ debug now TRUE":answ="+ debug now FALSE";
    }

    if (text == "/b1") {
      Button(1);
      answ="+Ok /b1";
    }

    if (text=="/t"){
      //answ = "+Ok "+hour+(String)":"+mins+(String)":"+sec+(String)" "+day+(String)"."+month+(String)"."+year+(String)" "+wd;
      answ = "+Ok time: "+getTimestr();    
      }

    if (text == "/b2") {
      Button(2);
      answ="+Ok /b2";
    }
    /*
    if (text.indexOf("=")==0) {
      answerbot(chat_id, text);      
    }*/
    if (text.indexOf("/bud")==0) {
      String sa = getValue(text,' ',1);
      String mess="\n";
      if (sa=="?"){
        int tph = getbud(0);
        int tpm = getbud(1);        
        mess+="is "+String(tph)+":"+String(tpm);
      }else{
      String sh = getValue(text,' ',1);
      String sm = getValue(text,' ',2);
      setbud(0, sh.toInt(),sm.toInt());
      mess+="set to "+sh+':'+sm;
      }
      answ="+Ok /bud "+mess;
    }

    if (text == "/u") {
      answ="+Ok uptime: "+millis2time();
    } 

    if (text == "/beep") {
      longbeep();
      answ="+Ok /beep";
    }
    if (text == "/on") {
      screenon();
      answ="+Ok /on screen";
    }
    if (text == "/off") {
      screenoff();
      answ="+Ok /off screen";
    }
    if (text == "/chon") {
      chimeon();
      answ="+Ok /on chime";
    }
    if (text == "/choff") {
      chimeoff();
      answ="+Ok /off chime";
    }
    if (text == "/chat") {
      answ="+Ok /chat="+chat_id;
    }
    /*
    if (getValue(text,' ',0) == "/int") {
      String sa = getValue(text,' ',1);
      int q=sa.toInt();
      myButtonReInit(q);
      beep(250,125);
      bot.sendMessage(chat_id, from_name+" "+sa, "");
    }
    */
   /*
    if (text == "/status") {
      String mess="";
      mess+="+Button 0 - "+String(getButton(0))+"\n";
      mess+="+Button 1 - "+String(getButton(1))+"\n";
      mess+="+Button 2 - "+String(getButton(2))+"\n";
      answ="+Ok /status";
    }
    */ 
    if (text == "/reboot") {
      String mess=F("+System is going to reboot NOW!");
      Treboot.setInterval(60*1000);
      Treboot.start();
      answ="+Ok system is going to reboot after 1 minute";
    }

    if (text == "/?") {
      String welcome = "Arduino 8266 UNO Bot, " + from_name + ".\n";
      welcome += "/b1 : to switch the button N\n";
      welcome += "/beep : to beep\n";
      welcome += "/u : uptime\n";
      welcome += "/chat : to return chat_id\n";
      welcome += "/bud ? : to see ringer\n";
      welcome += "/bud : to reset ringer\n";
      welcome += "/bud 18 00 : to set ringer\n";
      welcome += "/status : Returns current status of buttons\n";
      answ="+Ok /start";
    }
   return answ; 
 }


void handleNewMessages(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "UNKNOWN";
    if (debug){
      bot.sendMessage(myTele, "D:>"+text, "");      
    }
    answerbot(chat_id,text);
   }
 } 

void MyTeleBotInit(void){
  client.setInsecure(); // иначе не соединяется без этой команды
 } 

void goBot(void){
    if (millis() > Bot_lasttime + (unsigned int)Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  } 
 }

#endif