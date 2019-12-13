#define lcd1602
#ifdef Lz428266ZV
#ifndef defs
 #include "defs.h"
 #endif
#endif 
#ifndef common
  #include "common.h"
 #endif 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int acnt;
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

void showtime(void){
  lcd.setCursor(0, 1);
  if (hour<10){lcd.print("0");}
  lcd.print(hour);
  lcd.print(":");
  if (mins<10){lcd.print("0");}
  lcd.print(mins);
  lcd.print(":");
  if (sec<10){lcd.print("0");}
  lcd.print(sec);
 }

void screenon(void){}
void screenoff(void){}
void initLCD1602(){
  lcd.begin(5, 4);
  lcd.createChar(1, s1);  
  lcd.createChar(2, s2);
  lcd.createChar(3, s3);  
  lcd.createChar(4, s4);
  lcd.backlight();
  lcd.setCursor(0, 0);   
  lcd.print("Hello!");
  lcd.setCursor(0, 1);   
  lcd.print("Device starts!");
}

void tickclock(void){
  switch(acnt){
    case 0: lcd.print("\2");acnt+=1;break;
    case 1: lcd.print("\3");acnt+=1;break;
    case 2: lcd.print("\4");acnt+=1;break;
    case 3: lcd.print("\1");acnt+=1;break;
    case 4: lcd.print("\2");acnt+=1; acnt=1;break;
   }
}