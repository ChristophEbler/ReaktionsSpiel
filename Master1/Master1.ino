#include "FastLED.h"
#include <UTFT.h>
#include <ITDB02_Touch.h>
#define NUM_LEDS 16
#define DATA_PIN 8
// Declare which fonts we will be using
extern uint8_t BigFont[];
//extern uint8_t SevenSegNumFont[];

// Uncomment the next two lines for the ITDB02 Shield
UTFT          myGLCD(CTE70, 38, 39, 40, 41);
ITDB02_Touch  myTouch(6, 5, 4, 3, 2);

CRGB leds[NUM_LEDS];

int x, y;
char buf[10];
long maxtime = 60000;
unsigned long timer;
long Time =maxtime; 
bool start =false;
bool inTime = true;
bool BlinkShow =false;
bool OldBlinkShow =false;


void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin (9600);
// Initial setup
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();

  myTouch.InitTouch(LANDSCAPE);
  //myTouch.setPrecision(PREC_LOW);
  //myTouch.setPrecision(PREC_MEDIUM);
  //myTouch.setPrecision(PREC_EXTREME);
  myTouch.setPrecision(PREC_HI);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0,0);
  
  

    myGLCD.setColor(0, 255, 0); 
   myGLCD.drawRoundRect (10, 200, 790, 320);
   myGLCD.print("Punkte", 20, 180);
   
// Draw the lower row of buttons
  myGLCD.setColor(0, 255, 0);
  myGLCD.fillRoundRect (10, 370, 250, 470);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 370, 250, 470);
   myGLCD.setBackColor(0, 255,0);
  myGLCD.print("Neues Spiel", 50, 410);

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect (280, 370,510, 470);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (280, 370, 510, 470);
  myGLCD.setBackColor(255, 0,0);
  myGLCD.print("Spiel Stoppen", 300, 410);

  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (550, 370,790, 470);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0,255);
  myGLCD.drawRoundRect (550, 370, 790, 470);
  myGLCD.print("Blink", 630, 410);
  myGLCD.setBackColor(0, 0,0);
  
}


// Draw a red frame while a button is touched

void loop()
{
 
    if (myTouch.dataAvailable())
    {
      myTouch.read();
     y=myTouch.getX();
      x=myTouch.getY();
      Serial.print(x);
      Serial.print("\t");
      Serial.println(y);
      

     if (y > 270&&y<470)
     {
      if(x>0 && x<60)
      {if (Time ==maxtime)
      {
         timer =millis();
        start =true;
      }
      }

       if(x>80 && x<150)
      {
        Time = maxtime;
        start =false;
      }
      if(x>180 && x<245)
      {
        if (OldBlinkShow == false)
        {
          BlinkShow = true;
        }
        if (OldBlinkShow == true)
        {
          BlinkShow = false;
        }
        OldBlinkShow =BlinkShow;
        Serial.println(BlinkShow);
        start =false;
      }
     }
    }

    
    if (start == true&&Time >0)
    {
     
     Time  = maxtime-(millis()-timer);
  
    }
    if (Time <=0)
     {
      Time =0;
      inTime = false;
    //   myGLCD.setColor(0,0, 0); 
    //   myGLCD.fillRoundRect (55, 80,150, 120);
     }
     else 
     {
      inTime = true ;
      }
   if(inTime == true )
   {
     myGLCD.setColor(0, 255, 0); 
     myGLCD.drawRoundRect (10, 40, 790, 160);
     myGLCD.print("Zeit", 20, 10);
   }
    else 
    {
       myGLCD.setColor(255, 0, 0); 
   myGLCD.drawRoundRect (10, 40, 790, 160);
   myGLCD.print("Zeit", 20, 10);
     }
    ltoa(Time, buf, 10); 
    myGLCD.print(buf, 40,100);
     if (BlinkShow == true)
     {
       
      leds[0].setRGB( 0, 0,255);
      leds[1] = leds[0];
      FastLED.show();
      delay(500);
      leds[0].setRGB( 0, 255,0);
      leds[1] = leds[0];
      FastLED.show();
      delay(500);
      leds[0].setRGB( 255, 0,0);
      leds[1] = leds[0];
      FastLED.show();
      delay(500);
      }
     if (BlinkShow == false)
     {
   leds[0].setRGB( 0, 0, 0);
   leds[1] = leds[0];
 
          FastLED.show();
         delay(300);
      }
}

