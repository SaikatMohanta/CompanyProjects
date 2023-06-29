#include "EmonLib.h"
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27 //I2C adress, you should use the code to scan the adress first (0x27) here

#define BACKLIGHT_PIN 3 // Declaring LCD Pins
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

EnergyMonitor emon1;
EnergyMonitor emon2;

const int relay3 = 8; //master relay
const int relay1 = 9; //the live of primary supply is connected between NC and COM of relay 1 
const int relay2 = 10; //the nutral of primary supply is connected in between NC and COM of relay 2
//the live of secondary supply is connected in between NO and COM of relay 1
//the nutral of secondary supply is connected in between NO and com of relay 2 

void setup()
{  
  Serial.begin(9600);

  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH); //Lighting backlight
  lcd.home();
  lcd.clear();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  
  emon1.voltage(A0, 234.26, 1.7);
  emon2.voltage(A1, 234.26, 1.7);
  
}

void loop()
{
  emon1.calcVI(20,2000);
  emon1.serialprint();

  emon2.calcVI(20,2000);
  emon2.serialprint();
  
  float primary   = emon1.Vrms;             //extract Vrms into Variable
  float secondary   = emon2.Vrms;

  lcd.setCursor(0,0);
  lcd.print("1. ");
  lcd.setCursor(3,0);
  lcd.print(primary);
  lcd.setCursor(9,0);
  lcd.print("V");
  
  lcd.setCursor(0,1);
  lcd.print("2. ");
  lcd.setCursor(3,1);
  lcd.print(secondary);
  lcd.setCursor(9,1);
  lcd.print("V");
  

  if( primary > 200 && primary < 240 )//if primary supply is above 200V and less than 240V
  {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    //primary supply is connected to the load
    digitalWrite(relay3, LOW);
    //master relay is ON
  }

  else if( primary < 200 || primary > 240)//if primary supply is less than 200V or greater than 240V
  {
    if( secondary > 200 && secondary < 240 )//checks wheather secondary supply is greater than 200V and less tahn 240V
    {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      //secondary supply is connected to load
      digitalWrite(relay3, LOW);  
    }

    else if( secondary < 200 || secondary > 240)//checks wheather secondary supply is less than 200V or greater than 240V
    {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      //primary supply is connected to load
      digitalWrite(relay3, HIGH); 
      //but master relay is off, so load is off
    }
  }
}
