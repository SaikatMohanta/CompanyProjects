#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
/*
#define I2C_ADDR 0x27 //I2C adress, you should use the code to scan the adress first (0x27) here

#define BACKLIGHT_PIN 3 // Declaring LCD Pins
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
*/
LiquidCrystal_I2C lcd(0x27,16,2);
DateTime now;
RTC_DS3231 rtc;
char key;
int yar = 0;
int manth = 0;
int dete = 0;

int hh = 0;
int mm = 0;
int ss = 0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {7,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3,2,1,0}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    
void showDate(void);
void showTime(void);
void showDay(void);

void setup()
{
  //Serial.begin(9600); //Serial used
  lcd.begin();
  //lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.noBacklight(); //Lighting backlight
  //lcd.home ();   

  if (! rtc.begin()) 
  {
    lcd.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    lcd.println("RTC lost power");
    
  } 

}

void loop()
{
  key = kpd.getKey();
  if(key)
  { 
    switch(key)
    {
      case NO_KEY:
                  break;
    
   case 'A':
            settime();
            break;

   case '#':
            now = rtc.now();
            showDate();
            showTime();
            break;  
   }
  }

  now = rtc.now();
  showDate();
  showTime();
  
}


int GetNumber()
{
   int num = 0;
   char key = kpd.getKey();
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            //lcd.home();
            lcd.print(key);
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            lcd.clear();
            lcd.home();
            break;
      }

      key = kpd.getKey();
   }

   return num;
}

void showDate()
 {
  lcd.setCursor(0,0);
  //lcd.print("Date:");
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
 }

 void showTime()
 {
  lcd.setCursor(0,1);
  //lcd.print("Time:> ");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
} 

void settime()
{
  lcd.clear();
   lcd.home();
   lcd.print("set year: ");
   yar = GetNumber();

   lcd.clear();
   lcd.home();
   lcd.print("set month: ");
   manth = GetNumber();

   lcd.clear();
   lcd.home();
   lcd.print("set date: ");
   dete = GetNumber();

  lcd.clear();
   lcd.home();
   lcd.print("set hour: ");
   hh = GetNumber();

   lcd.clear();
   lcd.home();
   lcd.print("set min: ");
   mm = GetNumber();

   lcd.clear();
   lcd.home();
   lcd.print("set sec: ");
   ss = GetNumber();
   lcd.clear();
   rtc.adjust(DateTime(yar, manth, dete, hh, mm, ss));

   
}
