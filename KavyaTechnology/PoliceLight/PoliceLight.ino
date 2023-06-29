#include "avr/sleep.h"
#include <ezButton.h>

#define REDA 00
#define BLUEA 01
#define REDB 02
#define BLUEB 03

int buttonState;             
int lastButtonState = HIGH;

/*
//AS PER BANGALORE SMD PCB
const int LED_RedA=6,LED_BlueB=7;
const int LED_RedB=8,LED_BlueA=9;
const int LED=10;
*/

//AS PER BARASAT THT PCB
const int LED_RedA=6,LED_BlueA=7;
const int LED_RedB=8,LED_BlueB=9;
const int LED=10;

int btn_count=0;

byte whichLED1 = REDA;
byte whichLED2 = BLUEA;

byte Red_StateA = LOW;    
byte Blue_StateA = LOW; 
byte Red_StateB = LOW;    
byte Blue_StateB = LOW;

unsigned long switchDelay = 250;
unsigned long strobeDelay = 50;
 
unsigned long strobeWait = strobeDelay;
unsigned long waitUntilSwitch = switchDelay;

ezButton button(5);

void setup() 
{
  //declearing all the unused I/O as input pullup
  //to reduce the sleep mode power consumption
  pinMode(2, INPUT_PULLUP);   //unused
  pinMode(3, INPUT_PULLUP);   //unused
  pinMode(4, INPUT_PULLUP);   //unused
  pinMode(11, INPUT_PULLUP);   //unused 
  pinMode(12, INPUT_PULLUP);    //unused
  pinMode(13, INPUT_PULLUP);    //unused
  pinMode(14, INPUT_PULLUP);  //unused
  pinMode(15, INPUT_PULLUP);  //unused
  pinMode(16, INPUT_PULLUP);  //unused

  pinMode(LED_RedA, OUTPUT);
  pinMode(LED_BlueA, OUTPUT);
  pinMode(LED_RedB, OUTPUT); 
  pinMode(LED_BlueB, OUTPUT);
  
  pinMode(LED, OUTPUT);

  button.setDebounceTime(25);
  button.setCountMode(COUNT_FALLING);
}


void loop()
{
  button.loop();
  int reading = button.getStateRaw();
  if (reading != lastButtonState) 
  {
    btn_count = button.getCount();
  }


  switch(btn_count)
   {
    case 1:   //left strobe
        digitalWrite(LED_RedA, Red_StateA);     
        digitalWrite(LED_BlueA, Blue_StateA);
        digitalWrite(LED_RedB, 0);
        digitalWrite(LED_BlueB, 0);
        if ((long)(millis() - waitUntilSwitch)>=0) 
        {
            Red_StateA = LOW;
            Blue_StateA = LOW;
            whichLED1 = !whichLED1;  
            waitUntilSwitch += switchDelay;
        }
    
    
        if ((long)(millis() - strobeWait)>=0) 
        {
            if (whichLED1 == REDA)
                Red_StateA = !Red_StateA;
            if (whichLED1 == BLUEA)
                Blue_StateA = !Blue_StateA;
            strobeWait += strobeDelay;
        }   
          break;
          
    case 2:   //right strobe
        digitalWrite(LED_RedA, 0);
        digitalWrite(LED_BlueA, 0);
        digitalWrite(LED_RedB, Red_StateA);    
        digitalWrite(LED_BlueB, Blue_StateA);
        if ((long)(millis() - waitUntilSwitch)>=0) 
        {
        
            Red_StateA = LOW;
            Blue_StateA = LOW;
            whichLED1 = !whichLED1;  
            waitUntilSwitch += switchDelay;
        }
    
    
    if ((long)(millis() - strobeWait)>=0) 
    {
        if (whichLED1 == REDA)
            Red_StateA = !Red_StateA;
        if (whichLED1 == BLUEA)
            Blue_StateA = !Blue_StateA;
        strobeWait += strobeDelay;
    }
         break;
         
    case 3:   //left + right strobe
          digitalWrite(LED_RedA, Red_StateA);    
          digitalWrite(LED_BlueA, Blue_StateA);    
          digitalWrite(LED_RedB, Red_StateA);
          digitalWrite(LED_BlueB, Blue_StateA);
          
          if ((long)(millis() - waitUntilSwitch)>=0) 
          {
              
              Red_StateA = LOW;
              Blue_StateA = LOW;
              whichLED1 = !whichLED1; 
              waitUntilSwitch += switchDelay;
          }
          
          
          if ((long)(millis() - strobeWait)>=0) 
          {
              if (whichLED1 == REDA)
                  Red_StateA = !Red_StateA;
              if (whichLED1 == BLUEA)
                  Blue_StateA = !Blue_StateA;
              strobeWait += strobeDelay;
          } 
         break;
         
    case 4:  //torch on
          digitalWrite(LED,1);
          digitalWrite(LED_BlueA,0);
          digitalWrite(LED_BlueB,0);
          digitalWrite(LED_RedA,0);
          digitalWrite(LED_RedB,0);
          break;
          
    case 5:   //all off
          digitalWrite(LED,0);
          digitalWrite(LED_BlueA,0);
          digitalWrite(LED_BlueB,0);
          digitalWrite(LED_RedA,0);
          digitalWrite(LED_RedB,0);
          sleep(); //function to put the MCU into sleep mode
          button.resetCount();
          break;
          
    default:    //by default everything stays off
          //btn_count=0;
          
          digitalWrite(LED,0);
          digitalWrite(LED_BlueA,0);
          digitalWrite(LED_BlueB,0);
          digitalWrite(LED_RedA,0);
          digitalWrite(LED_RedB,0);
          break;
   }
}


void sleep()
{
    
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();  
    sei();                                    
    sleep_cpu();                              
    sleep_disable(); 
    //cli();
    
}
