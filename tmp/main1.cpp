#include <Arduino.h>
#include "LowPower.h"
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include "GyverUART.h";
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>


// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//https://alexgyver.ru/lessons/arduino-libraries/

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define PORTRAIT 0
#define LANDSCAPE 1
#define PORTRAIT_REV 2
#define LANDSCAPE_REV 3


//#include <SPI.h>          // f.k. for Arduino-1.5.2

//#include <SD.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//File myFile;



// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ORANGE  0xFD20

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

int i=0;
int tw;
int th;
int prevx=0;
int prevy=0;
int x=0;
int y=0;
int step=1;
int speed = 0;
char cstr[80];
String inString="";
String g="";
/*
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xBF){
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB7; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
*/

void refresh(int value, byte ms_delay)
{
  static float ltx = 0;    // Saved x coord of bottom of needle
  static uint16_t osx = 160, osy = 160; // Saved x & y coords0
  static int old_analog =  -999; // Value last displayed
   
    float sdeg = map(old_analog, -10, 110, -150, -30); 

    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    float tx = tan((sdeg+90) * 0.0174532925);

    ltx = tx;
    osx = (sx * 130) + 160;
    osy = (sy * 130) + 187;

    //tft.drawLine(160 + (27 * ltx) - 1, 187 - 27, osx - 1, osy, BLACK);
    //tft.drawLine(160 + (27 * ltx), 187 - 27, osx, osy, BLACK);
    //tft.drawLine(160 + (27 * ltx) + 1, 187 - 27, osx + 1, osy, BLACK);

     sdeg = map(value, -10, 110, -150, -30); 

     sx = cos(sdeg * 0.0174532925);
     sy = sin(sdeg * 0.0174532925);

     tx = tan((sdeg+90) * 0.0174532925);

    ltx = tx;
    osx = (sx * 130) + 160;
    osy = (sy * 130) + 187;

    tft.drawLine(160 + (27 * ltx) - 1, 187 - 27, osx - 1, osy, RED);
    tft.drawLine(160 + (27 * ltx), 187 - 27, osx, osy, MAGENTA);
    tft.drawLine(160 + (27 * ltx) + 1, 187 - 27, osx + 1, osy, RED);
    old_analog=value;
}

void analogMeter()
{
  const  char labels[6][4] = {"  0", " 25", " 50", " 75", "100", "%DC"};
  // Meter outline
  tft.fillRect(0, 0, 319, 168, LIGHTGREY);
  tft.fillRect(5, 3, 309, 162, WHITE);  
  tft.setTextColor(BLACK);  // Text colour
  
  
  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for( int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 20;   
    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (133 + tl) + 160;
    uint16_t y0 = sy * (133 + tl) + 187;
    uint16_t x1 = sx * 133 + 160;
    uint16_t y1 = sy * 133 + 187;
    
    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (133 + tl) + 160;
    int y2 = sy2 * (133 + tl) + 187;
    int x3 = sx2 * 133 + 160;
    int y3 = sy2 * 133 + 187;

    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x3, y3, x1, y1, BLACK);

    // Yellow zone limits
    if((i >= -50) && (i < 0)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, YELLOW);
      }

    // Green zone limits
    if((i >= 0) && (i < 25)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, GREEN);
      }

    // Orange zone limits
    if((i >= 25) && (i < 50)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ORANGE);
      }
    
    // Short scale tick length
    if (i % 25 != 0) {
      tl = 11; 
      // Recalculate coords incase tick lenght changed
      x0 = sx * (133 + tl) + 160;
      y0 = sy * (133 + tl) + 187;
      }
    // Draw tick
    tft.drawLine(x0, y0, x1, y1, BLACK);
    
    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (133 + tl + 10) + 160 -18; //corr
      y0 = sy * (133 + tl + 10) + 187;
      int labp = (i / 25) + 2;
      switch (labp) {
        case 0: tft.setCursor( x0, y0-16); break;
        case 1: tft.setCursor( x0, y0-12); break;
        case 2: tft.setCursor( x0, y0 -8); break;
        case 3: tft.setCursor( x0, y0-12); break;
        case 4: tft.setCursor( x0, y0-16); break;
      }
    tft.setFont();  
    tft.setTextColor(BLACK);  
    tft.setTextSize(2);
    tft.println(labels[labp]);
    }
  }  
}
/*------------------------------------------------------*/

void showbar(int speed){
    int col=GREEN;
    if (speed<80) col=YELLOW;
    if (speed<50) col=ORANGE;
    if (speed<30) col=RED;
    if (speed<0) return;
    tft.fillRect(tft.width()-103, 1, 102, 21, DARKGREY);
    tft.fillRect(tft.width()-102, 2, speed, 19, col);
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, int col,  const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(col);
    tft.setTextSize(sz);
    tft.print(msg);
}

void setup(void) 
{
    uartBegin(9600); 
    uint16_t ID = tft.readID(); //
    uartPrintln(ID, HEX);
    tft.begin(ID);  // my is 9327
    //tft.cp437(true);
    tft.setRotation(LANDSCAPE);
    tft.fillScreen(BLACK);
    tft.setFont();
    tft.setTextSize(2);
    i=0;
    
 uartPrintln(tw);
 uartPrintln(th);
 tft.drawFastHLine(0,0,tft.width()-1,GREEN) ;// ok
 tft.drawFastHLine(0,tft.height()-1,tft.width()-1,GREEN) ;// ok
 tft.drawFastVLine(0,0,tft.height()-1,GREEN) ;// ok
 tft.drawFastVLine(tft.width()-1,0,tft.width()-1,GREEN) ;// ok
 i=1;
}

void loop(void) 
{  

  while (uartAvailable() > 0) {
    char inChar = uartRead();
    inString += inChar;
    if (inChar == '\n') {
  switch (inString.charAt(0))
{
case '0':
    g=inString.substring(2,80);
    speed=g.toInt();
    analogMeter();
    refresh(speed,0);
  break;  
case '1':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,1, 95, 50, BLACK);//190
    showmsgXY(1, 51, 1, &FreeSevenSegNumFont,GREEN, cstr);
  break;
case '2':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(96,1, 95, 50, BLACK);
    showmsgXY(96, 51, 1, &FreeSevenSegNumFont,YELLOW, cstr);
  break;
case '3':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,52, 95, 50, BLACK);
    showmsgXY(1, 101, 1, &FreeSevenSegNumFont,RED, cstr);
  break;
case '4':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(96,52, 95, 50, BLACK);
    showmsgXY(96, 101, 1, &FreeSevenSegNumFont,CYAN, cstr);
  break;
case '5':
    g=inString.substring(2,80);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,99, 398, 22, BLACK);
    showmsgXY(2, 119, 1,&FreeSans12pt7b,GREEN, cstr);
  break; 
case '6':
    g=inString.substring(2,80);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,119, 398, 22, BLACK);
    showmsgXY(2, 139, 1,&FreeSans12pt7b,GREEN, cstr);
  break;
case '7':{
    g=inString.substring(2,80);
    int atn=g.toInt();// num of bars
    tft.fillScreen(BLACK);
    while(true){
      //int mw=;//
      int mh=tft.height(); 
      int wb=tft.width()/atn;// bar width
      int th;
      //tft.fillScreen(BLACK);
      for(int r=0;r<atn;r++){
        th=random(150);
        tft.fillRect(r*wb,0,1,mh,RED);
        delay(5);
        tft.fillRect(r*wb,0,wb-1,mh,BLACK);
        tft.fillRect(r*wb,mh-th,wb-1,mh,GREEN);
        //int num = 1234;
        char acstr[16];
        itoa(r, acstr, 10);
        tft.fillRect(0,0,50,30,BLACK);
        showmsgXY(1, 20, 1, &FreeSans9pt7b,YELLOW,  acstr);
        //delay(50);
      }
      //delay(1000);
      if (uartAvailable()) break;
    }
  break;}
case '8':
    g=inString.substring(2,80);
    speed=g.toInt();
    showbar(speed);
  break;
case '9':
    speed=0;
    while(true){
    float al=(270-speed)*PI/180;    
    int yc=(tft.height() / 2)-1;
    int xc=(tft.width() / 2)-1;
    int r=80;
    int dx=r*sin(al);
    int dy=r*cos(al);
    
    tft.writeLine(xc-1,yc,xc+prevx-1, yc+prevy,  BLACK);
    tft.writeLine(xc+1,yc,xc+dx+1, yc+dy,  RED);

    tft.writeLine(xc,yc,xc+prevx, yc+prevy,  BLACK);
    tft.writeLine(xc,yc,xc+dx, yc+dy,  MAGENTA);

    tft.writeLine(xc+1,yc,xc+prevx+1, yc+prevy,  BLACK);
    tft.writeLine(xc-1,yc,xc+dx-1, yc+dy,  RED);
    
    step=2;
    speed+=step;
    prevx=dx;
    prevy=dy;
    if (uartAvailable()) {
      tft.writeLine(xc-1,yc,xc+prevx-1, yc+prevy,  BLACK);
      tft.writeLine(xc,yc,xc+prevx, yc+prevy,  BLACK);
      tft.writeLine(xc+1,yc,xc+prevx+1, yc+prevy,  BLACK);
      break;
    }
    delay(10);
    showbar(abs(speed %100));
    if (speed>32767) speed=0;
    }
  break; 
case '+':
 step+=2;
 break;
case '-':
 step-=2;
 break;                       
default:
  break;
}
 inString = "";
}
}
//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
//g=inString.substring(2,80);
//    int atn=g.toInt();// num of bars
int atn=48;
    tft.fillScreen(BLACK);
    while(true){
      //int mw=;//
      int mh=tft.height(); 
      int wb=tft.width()/atn;// bar width
      int th;
      //tft.fillScreen(BLACK);
      int tprev=100;
      for(int r=0;r<atn;r++){
        th=random(150);
        //tft.fillRect(r*wb,0,1,mh,RED);
        delay(20);
        tft.fillRect(r*wb,0,wb-1,mh,BLACK);
        tft.fillRect(r*wb,0,wb-1,mh-th-1,RED);
        tft.fillRect((r-1)*wb,0,wb-1,mh-tprev,BLACK);
        tft.fillRect(r*wb,mh-th,wb-1,mh,GREEN);
        //int num = 1234;
        char acstr[16];
        itoa(th, acstr, 10);
        tft.fillRect(0,0,50,30,BLACK);
        showmsgXY(1, 20, 1, &FreeSans9pt7b,YELLOW,  acstr);
        delay(250);
        tprev=th;
        if (r==atn-1){tft.fillRect((r)*wb,0,wb-1,mh-th,BLACK);}
      }
      //delay(1000);
      if (uartAvailable()) break;
    }  
}

