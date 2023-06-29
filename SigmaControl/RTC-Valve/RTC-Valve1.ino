#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Keypad.h>
#include<Wire.h>
const byte ROWS = 4; 
const byte COLS = 4; 
char Keys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad mykey = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

DateTime now;
char days[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2);

void showDate(void);
void showTime(void);
void showDay(void); 

void setup ()
{
  Serial.begin(9600);
  lcd.begin();
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop () 
  {
  now = rtc.now();
  showDate();
  showDay();
  showTime();
}
 void showDate()
 {
  lcd.setCursor(0,0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
 }
 void showDay()
 {
  lcd.setCursor(12,0);
  lcd.print(days[now.day()]);
 }
 void showTime()
 {
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
} 
