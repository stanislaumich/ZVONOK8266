#include <Arduino.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
// NTP
#include <WiFiUdp.h>

unsigned int localPort = 2390;            // local port to listen for UDP packets
IPAddress timeServer(129, 6, 15, 28);     // time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48;           // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];      // buffer to hold incoming and outgoing packets

WiFiUDP udp;                              // A UDP instance to let us send and receive packets over UDP
boolean doNTP=false;


unsigned long sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;

  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
/*
if(doNTP) {
    sendNTPpacket(timeServer);            // send an NTP packet to a time server
    delay(1000);                          // wait to see if a reply is available

    int cb = udp.parsePacket();           // get packet (if available)
    if (!cb) {
      Serial.println("No packet yet");
    } else {
      udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unsigned long epoch = secsSince1900 - seventyYears;
      doNTP=false;
     }
  }
*/
// NTP


/*IO index 	ESP8266 pin 	IO index 	ESP8266 pin
0  	GPIO16 	7 	GPIO13
1 	GPIO5 	8 	GPIO15
2 	GPIO4 	9 	GPIO3
3 	GPIO0 	10 	GPIO1
4 	GPIO2 	11 	GPIO9
5 	GPIO14 	12 	GPIO10
6 	GPIO12 		*/

//const char* ssid = "ZTE54";
//const char* password = "12121111";
const char* ssid = "Direktor";
const char* password = "2932802951";
const char* assid = "Direktor1";
const char* apassword = "2932802951";
//MDNSResponder mdns;
HTTPClient http;
ESP8266WebServer server(80);

int offset = 2;


char weekDay[][4] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" };

byte d[8][6];

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

DateTime dt(2016, 12, 02, 12, 03, 0, 5);

uint8_t s1[8] =
{
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000
};
uint8_t s2[8] =
{
  B00000,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};
uint8_t s3[8] =
{
  B00000,
  B00001,
  B00010,
  B00100,
  B01000,
  B10000,
  B00000
};
uint8_t s4[8] =
{
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};

byte curhour[6];
byte curday[40];

byte ee[512]=
{
  8,0,8,5,10,      8,10,8,15,10,
  8,20,8,25,15,    8,30,8,35,15,
  8,40,8,45,25,    8,50,8,55,10,
  9,0,9,5,10,      9,10,9,15,0,

  14,0,14,5,10,      14,10,14,15,10,
  14,20,14,25,15,    14,30,14,35,15,
  14,40,14,45,25,    14,50,14,55,10,
  15,0,15,5,10,      15,10,15,15,0,

  8,3,8,45,10,     8,55,9,40,10,
  9,50,10,35,15,   10,50,11,35,15,
  11,50,12,35,25,  13,0,13,45,10,
  13,55,14,40,10,  14,50,15,35,0,

  8,4,8,45,15,     9,0,9,45,10,
  9,55,10,40,10,   10,50,11,35,5,
  11,40,12,25,20,  12,45,13,30,10,
  13,40,14,25,10,  14,35,15,20,10,

  8,5,8,45,15,     9,0,9,45,10,
  9,55,10,40,10,   10,50,11,35,5,
  11,40,12,25,20,  12,45,13,30,10,
  13,40,14,25,10,  14,35,15,20,10,

  8,6,8,45,15,     9,0,9,45,10,
  9,55,10,40,10,   10,50,11,35,5,
  11,40,12,25,20,  12,45,13,30,10,
  13,40,14,25,10,  14,35,15,20,10,

  0,0,0,192,168,1,
  122,9
};

int address = 0;
byte value;
int j = 0;

long previousMillis = 0;
long previousMillisZ = 0;

long interval = 1000; // обновление экрана миллисекунд
long intervalZ = 9; // длительность звонка сек
long cintervalZ = 9; // длительность звонка сек по умолчанию если не смог считать
long conntimeout = 30000; // сколько пытаться подключаться к сети
int beeplen=15;

const int led = 12;
const int redpin=15;
const int zvpin=16;//GPIO
const int soundpin=14; //14 norm// 15 RED, 14 - рядом маленький 16 - рядом ещё ближе  13 - синий 12 зелёный
const int bluepin=13;
const int greenpin=12;

bool zvstat;

String message;
///////////////////////////////////// SERVER
IPAddress ip(192, 168, 1, 122);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(194, 158, 196, 137);
IPAddress dns2(194, 158, 196, 141);


// SERVER //////////////////////////////////////////////////////////////////////////////
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "http://192.168.1.14/get?a=1");
  digitalWrite(led, 0);
}////////////////////////////////////////////////////////////////
void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}/////////////////////////////////////////////////////////////////////
void handleSet(){
  digitalWrite(led, 1);
  message = "<html><head></head><body>";
  if (server.arg(0).toInt()==1){// установка времени звонка для урока в этот день
  int i=(server.arg(1).toInt()-1)*40+(server.arg(2).toInt()-1)*5;
  ee[i+0]=server.arg(3).toInt();
  ee[i+1]=server.arg(4).toInt();
  ee[i+2]=server.arg(5).toInt();
  ee[i+3]=server.arg(6).toInt();
  ee[i+4]=server.arg(7).toInt();
  message += "http://IP/set?act=1&d=1&n=1&bh=1&bm=1&eh=1&em=1&pd=7<br>\nOK\n";
  //message += "bh "+  String(ee[i+0])+ "<br> ";
  //message += "bm "+  String(ee[i+1])+ "<br> ";
  //message += "eh "+  String(ee[i+2])+ "<br> ";
  //message += "em "+  String(ee[i+3])+ "<br> ";
  //message += "pd "+  String(ee[i+4])+ "<br> ";

}
  message+="</body></html>";
if (server.arg(0).toInt()==2){// установка времени звонка для урока в этот день
  message += "http://192.168.1.15/set?act=2<br>\n";
  writeee();
  message += "Settings are stored in EEPROM<br> ";
}
  server.send(200, "text/html", message);
  digitalWrite(led, 0);
}/////////////////////////////////////////////////////////////////////
void handleGet(){
  digitalWrite(led, 1);
 int gd=0;
  message = "";
    for (uint8_t i=(server.arg(0).toInt()-1)*40; i<(server.arg(0).toInt()-1)*40+40; i+=5){
      message +=   String(ee[i+0])+"\n";
      message +=   String(ee[i+1])+"\n";
      message +=   String(ee[i+2])+"\n";
      message +=   String(ee[i+3])+"\n";
      message +=   String(ee[i+4])+"\n";
    }
    message+="";

  server.send(200, "text/plain", message);
  digitalWrite(led, 0);
}////////////////////////////////////////////////////////////////////////
void handleTime(){
  digitalWrite(led, 1);
 //int gd=0;
 if (server.arg(0).toInt()==1){
  message = "Time set";
  int sd=server.arg(1).toInt();
  int sm=server.arg(2).toInt();
  int sy=server.arg(3).toInt();
  int sw=server.arg(4).toInt();
  int sth=server.arg(5).toInt();
  int stm=server.arg(6).toInt();
  int sts=server.arg(7).toInt();
  DateTime msdt(sy, sm, sd, sth, stm, sts, sw);

  rtc.setDateTime(msdt); //Adjust date-time as defined 'dt' above
}
if (server.arg(0).toInt()==2){
 DateTime now = rtc.now();
 message = "Current time = H M S D M Y WD\n" + String(now.hour())+ "\n" + String(now.minute()) + "\n" + String(now.second()) + "\n" + String(now.date()) + "\n" + String(now.month()) + "\n" + String(now.year()) + "\n" + String(now.dayOfWeek())+"\n";
 message+="\n http://IP/time?act=1&sd=d&sm=m&sy=y&sw=w&hour=h&min=m&sec=s to set new time, http://IP/time?act=2 to see current";
}

    /*for (uint8_t i=(server.arg(0).toInt()-1)*40; i<(server.arg(0).toInt()-1)*40+40; i+=5){
      message +=   String(ee[i+0])+"\n";
      message +=   String(ee[i+1])+"\n";
      message +=   String(ee[i+2])+"\n";
      message +=   String(ee[i+3])+"\n";
      message +=   String(ee[i+4])+"\n";
    }
    */
    message+="";

  server.send(200, "text/plain", message);
  digitalWrite(led, 0);
}////////////////////////////////////////////////////////////////////////



//- SERVER //////////////////////////////////////////////////////////////////////////////


void beep(unsigned char delayms){
 for (int i=0;i<delayms;i++){
  digitalWrite(soundpin,1);
  delayMicroseconds(500);
  digitalWrite(soundpin,0);
  delayMicroseconds(500);
 }
 setcurhour();
 DateTime now = rtc.now(); //get the current date-time
 Serial.print(now.date(), DEC);  Serial.print('.');
 Serial.print(now.month(), DEC); Serial.print('.');
 Serial.print(now.year(), DEC);  Serial.print(' ');

 Serial.print(now.hour(), DEC);  Serial.print(':');
 Serial.print(now.minute(), DEC);Serial.print(':');
 Serial.print(now.second(), DEC);Serial.print(' ');
 Serial.print(weekDay[now.dayOfWeek()]);Serial.println();
 //zon();
 //if (now.minute()%2==0){zvstat=true;} else {zvstat=false;}// zv every three minutes
 //zoff();
}

//  EEPROM READ WRITE  /////////////////////////////////////////////////////////////
int writeee(){
  EEPROM.begin(512);
  for(int i=0;i<512;i++){
   EEPROM.write(i, ee[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

int readee(){
 EEPROM.begin(512);
  for(int i=0;i<512;i++){
   ee[i]=EEPROM.read(i);
  }
 EEPROM.end();
}
// - EEPROM READ WRITE  /////////////////////////////////////////////////////////////
void printtime()
{
  DateTime now = rtc.now(); //get the current date-time

  lcd.setCursor(0, 0);
  lcd.print(curhour[0]);
  lcd.print(' ');
  if (curhour[1]<10){lcd.print('0');}
  lcd.print(curhour[1]);
  lcd.print(':');
  if (curhour[2]<10){lcd.print('0');}
  lcd.print(curhour[2]);
  lcd.print(' ');
  if (curhour[3]<10){lcd.print('0');}
  lcd.print(curhour[3]);
  lcd.print(':');
  if (curhour[4]<10){lcd.print('0');}
  lcd.print(curhour[4]);
  lcd.print(' ');
  if (curhour[5]<10){lcd.print('0');}
  lcd.print(curhour[5]);

  lcd.setCursor(0, 1);
  int hr=now.hour();
  if (hr<10){
    lcd.print('0');
  }
  lcd.print(hr, DEC);
  lcd.print(':');
  int min=now.minute();
  if (min<10){
    lcd.print('0');
  }
  lcd.print(min, DEC);
  lcd.print(':');
  int sec=now.second();
  if (sec<10){
    lcd.print('0');
  }
  lcd.print(sec, DEC);

  lcd.print(' ');
  lcd.print(weekDay[now.dayOfWeek()]);
  rtc.convertTemperature();
  lcd.print(' ');
  lcd.print(rtc.getTemperature());

  if (sec%10==0){beep(beeplen);setcurhour();}
  //if (sec%2==0) digitalWrite(15,LOW); else digitalWrite(15,HIGH);//red работает на пин 15
}


// WORK WITH RASP /////////////////////////////////////////////////////////////////
void setcurday(){
  DateTime now = rtc.now();
  if(now.dayOfWeek()>0){
  j = 0;
  int k=(now.dayOfWeek()-1)*40;
  for (int i=0; i<40; i+=1){
    curday[j] =   ee[k+i];
    j+=1;
  }
 }
}

void setcurhour(){
setcurday();
DateTime now = rtc.now();
int j=0;
bool ek=true;
int h=now.hour();
int m=now.minute();
int t=h*60+m;
int n=1;
while (j<40&&ek) {
  if (t<=curday[j+2]*60+curday[j+3]){
 curhour[0]=n;
 curhour[1]=curday[j+0];
 curhour[2]=curday[j+1];
 curhour[3]=curday[j+2];
 curhour[4]=curday[j+3];
 curhour[5]=curday[j+4];
 ek=false;
 }
 j+=5;
 n+=1;
}// while

}
// - WORK WITH RASP /////////////////////////////////////////////////////////////////
int tzv;

void zwork(){
 DateTime now = rtc.now();

 for (int i=0;i<36;i+=5){
 if((now.hour()*60+now.minute()==curday[i]*60+curday[i+1])
      ||(now.hour()*60+now.minute()==curday[i+2]*60+curday[i+3]))
 {
   if (now.second()<intervalZ){
   zon();
   delay(intervalZ*1000);
   zoff(); //выключаем звонок после его  таймаута
 }//if
 }// if
 }// for
/*
 if ((now.minute()%2==0)&&now.second()<intervalZ){
 zon();
 delay(intervalZ*1000);
 zoff(); //выключаем звонок после его  таймаута
}
*/
}// void zwork;

void zon(){
  //analogWrite(zvpin,1);
digitalWrite(zvpin,HIGH);
digitalWrite(greenpin,HIGH);
}
void zoff(){
  //analogWrite(zvpin,0);
digitalWrite(zvpin,LOW);
digitalWrite(greenpin,LOW);
}
/*
time_t getNtpTime() {
//switch offset every sync, so that we can se that a sync happened
//if(offset == 1) {
//  offset = 2;
//} else {
//  offset = 1;
//}
esp8266_ntp.setGMTOffset(offset);
return esp8266_ntp.getTime();
time_t t = getNtpTime();
    rtc.setDateTime(t);
}

*/
void myinit(){
  Serial.begin(115200);
  Serial.println("Starting!");

  pinMode(zvpin, OUTPUT);
  digitalWrite(zvpin,HIGH);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(soundpin, OUTPUT);
  digitalWrite(soundpin, LOW);
  //pinMode(15, OUTPUT);// 15 RED, 14 - рядом маленький 16 - рядом ещё ближе  13 - синий 12 зелёный
  //pinMode(14, OUTPUT);

  zoff();
  rtc.begin();
  //setSyncProvider(getNtpTime);
  //setSyncInterval(2*60*60);




  //writeee();
  readee();
  zvstat=false;
  intervalZ=ee[511];
  if (intervalZ==0){
   intervalZ=cintervalZ;
   ee[511]=intervalZ;
    writeee();
  }
  lcd.begin(5, 4);
  lcd.createChar(1, s1);  lcd.createChar(2, s2);
  lcd.createChar(3, s3);  lcd.createChar(4, s4);
  lcd.backlight();
  lcd.setCursor(0, 0);   lcd.print("Hello!");
  lcd.setCursor(0, 1);   lcd.print("ZVONOK Starts!");
  delay(1000);
  setcurhour();
 //////////////////////////////////////////////////

}

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(assid, apassword);
  WiFi.config(ip, gateway, subnet, dns1, dns2); // before or after Wifi.Begin(ssid, password);
  WiFi.begin();


  server.on("/",    handleRoot);
  server.on("/set", handleSet);

  server.on("/get", handleGet);
  server.on("/time", handleTime);
  //server.on("/fw", handleGet);
  server.onNotFound(handleNotFound);
 /*
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
 */
  server.begin();
  Serial.println("HTTP server started");
  beeplen=15;
}

void startWiFi(){
  WiFi.config(ip, gateway, subnet, dns1, dns2); // before or after Wifi.Begin(ssid, password);
  WiFi.begin(ssid, password);
  Serial.println("");
  lcd.setCursor(0, 0);
  lcd.print("Conn   ");
  lcd.print(ssid);
  lcd.print("      ");
  int cnt=0;
  // Wait for connection
  previousMillis = millis();
  bool kk=true;
  while ((WiFi.status() != WL_CONNECTED)&&kk) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
   switch(cnt){
    case 0: lcd.print("\2");cnt+=1;break;
    case 1: lcd.print("\3");cnt+=1;break;
    case 2: lcd.print("\4");cnt+=1;break;
    case 3: lcd.print("\1");cnt+=1;break;
    case 4: lcd.print("\2");cnt+=1; cnt=1;break;
   }
   lcd.print("               ");
   unsigned long currentMillis = millis();
   if(currentMillis - previousMillis > conntimeout){
    kk=false;
   }
  }
  if (!kk){beeplen=150;}



  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  lcd.print(" OK!       ");
  delay(1000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  IPAddress hostIP;
if (!WiFi.hostByName("rasp.lz42.ru", hostIP))
{
      Serial.println("Couldn't get IP for RASP");
      //return;
}
else{
      Serial.print("RASP.LZ42.RU IP=");
      Serial.println(hostIP);
}
  /*
   String hst;
   //hst= "http://rasp.lz42.ru/ips.php?ip=" + WiFi.localIP();
   hst= "ips.php?ip=" + WiFi.localIP();
   Serial.println(hst);
   //hostIP+="/ips.php?ip="+WiFi.localIP();
   //http.begin(hostIP,80); //HTTP
   //http.begin("192.168.41.1", 80, "/led1");
   http.begin("http://rasp.lz42.ru",80,hst);
   http.begin(hst);
   Serial.println(hst);
   int httpCode = http.GET();
   Serial.print(" Rasp answer: ");
   Serial.println(httpCode);
   http.end();
  /**/

  server.on("/",    handleRoot);
  server.on("/set", handleSet);

  server.on("/get", handleGet);
  server.on("/time", handleTime);
  //server.on("/fw", handleGet);
  server.onNotFound(handleNotFound);
 /*
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
 */
  server.begin();
  Serial.println("HTTP server started");

}




// START SETUP  ///////////////////////////////////////////////////////////////////////////
void setup ()
{
    myinit();
    zoff();
    startWiFiAP();
    //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above
    delay(1000);
}
// END - SETUP  ///////////////////////////////////////////////////////////////////////////


// START - LOOP  ////////////////////////////////////////////////////////////////////////////
void loop ()
{
zwork(); //обработка звонка вынесена за пределы паузы, пусть проверяет
// её дело включить переменную звонка и звонок и получить новое время звонка
// и после звонка погасить его
/*if (zvstat){zon();}
unsigned long currentMillisZ = millis();
if(currentMillisZ - previousMillisZ > intervalZ) {
 previousMillisZ = currentMillisZ;
 if (zvstat){zoff();}
 }
*/

//zonoff();
server.handleClient();
// этот участок выше работает без пауз

// участок ниже выполняется раз в интервал, обновление времени на экране
unsigned long currentMillis = millis();
if(currentMillis - previousMillis > interval) {
 previousMillis = currentMillis;
 printtime(); //вывод на экран подготовленной если надо инфы
 }

 //zon();delay(1000);zoff();
}// TOTAL END ///////////////////////////////////////////////////////////////////////
