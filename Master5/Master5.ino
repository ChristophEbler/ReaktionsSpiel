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
int phLed[8][2]={
  {0,1},
  {2,3},
  {4,5},
  {6,7},
  {8,9},
  {10,11},
  {12,13},
  {14,15},
};

int x, y;
char buf[10];
char buf1[10];
long maxtime = 60000;
long pauseTime =500;
long holdTime =1000;
unsigned long timer;
unsigned long blinkTimer;
int analogref =800;
unsigned long pausetimer;
unsigned long holdtimer;

long Time =maxtime; 

bool start =false;
bool inTime = true;
bool BlinkShow =false;
bool OldBlinkShow =false;
bool point =false;
bool pointflag = false;
bool pointLive = false;
bool firstPoint =false;
bool pointPressed =false;
bool pointAktiv =false;
bool nextPoint =false;

int startPointAdr = 9;
int aktPountAdr = 9;

long points =0;
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
  allOff();
}


// Draw a red frame while a button is touched

void loop()
{
 
    if (myTouch.dataAvailable())
    {
      myTouch.read();
     y=myTouch.getX();
      x=myTouch.getY();
    
      

     if (y > 270&&y<470)
     {
      if(x>0 && x<60)
      {if (Time ==maxtime)
      {
        //Startbedingugen
          points =0;
             ltoa(points, buf1, 10); 
           myGLCD.print( buf1, 40,250);
         
            
          firstPoint =true;
        generateStartPoint();
        delay(100);
      firstPoint =true;
         timer =millis();
        start =true;
     //   delay(100);
         allOff();
         point =true;
      }
      }

       if(x>80 && x<150)
      {
        Time = maxtime;
        start =false;
        allOff();
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
     
    myGLCD.setColor(0, 255, 0); 
   myGLCD.drawRoundRect (10, 200, 790, 320);
   myGLCD.print("Punkte", 20, 180);
     
   }
    else 
    {
       myGLCD.setColor(255, 0, 0); 
   myGLCD.drawRoundRect (10, 40, 790, 160);
   myGLCD.print("Zeit", 20, 10);
   
    myGLCD.setColor(255, 0, 0); 
   myGLCD.drawRoundRect (10, 200, 790, 320);
   myGLCD.print("Punkte", 20, 180);
     }
     //zeit 
    ltoa(Time, buf, 10);
     ltoa(points, buf1, 10); 
    
    myGLCD.print(buf, 40,100);
    myGLCD.print( buf1, 40,250);
    
    if (Time == 0)
    {
       myGLCD.print("0           ", 40,100);
      }
if (inTime == true&&start == true)
{
     if (nextPoint == true)
      {
       nextPoint = false;
       point =true;
       }
      
  if (point ==true)
  {
    pausetimer = millis();
     point =false;
     pointflag = true;
  }
  if ( (millis()-pausetimer > pauseTime&&pointflag == true))
  {
   
   aktPountAdr = generatePlayPoint();
   holdtimer = millis();
   pointAktiv =true;
    if (  firstPoint ==true)
    {
        firstPoint =false;
      }
      pointPressed = false;
    pointflag = false;
    }
if ((millis()-holdtimer >holdTime)&&pointAktiv)
{ 
  pointAktiv =false;
  
  nextPoint = true;
  
}
    if ((analogRead(aktPountAdr)>=analogref)&&inTime == true&&start == true &&  pointPressed ==false&& pointAktiv ==true)
    {
       pointPressed = true;
      points =points +1;
       
      ledSetCollor(aktPountAdr,0,255,0); 
      }
      if (pointAktiv ==false)
      {ledSetCollor(aktPountAdr,0,0,0); }
      
   
  }
    BlinkShowFunc(BlinkShow );

}
void BlinkShowFunc(boolean LBlinkShow )
{
       if (LBlinkShow == true)
     {
      delay (200);
       ledSetCollor(0,0,0, 255); 
      
      if ((millis() -blinkTimer)>1000)
      {
        ledSetCollor(1,255,0, 0);
      }
        if ((millis() -blinkTimer)>2000)
      {
        ledSetCollor(2,0,255, 0);
      }
        if ((millis() -blinkTimer)>3000)
      {
        ledSetCollor(3,0,0, 255);
      }
        if ((millis() -blinkTimer)>4000)
      {
        ledSetCollor(4,255,0, 0);
      }
        if ((millis() -blinkTimer)>5000)
      {
        ledSetCollor(5,0,255, 0);
      }
        if ((millis() -blinkTimer)>6000)
      {
        ledSetCollor(6,0,0, 255);
      }
       if ((millis() -blinkTimer)>7000)
      {
        ledSetCollor(7,255,255, 255);
         blinkTimer = millis();
      }
      
      

      
      }
     if (LBlinkShow == false)
     {
      blinkTimer = millis();
  
      delay(200);

      }
  } 
 void ledSetCollor(int nr,int r,int g, int b)
 {
  
    leds[ phLed[nr][0]].setRGB( g, r, b);
   leds[phLed[nr][1]] = leds[phLed[nr][0]];
 
          FastLED.show();
  
 }
 void generateStartPoint()
 {
  int stIndex = random(0, 8);
  startPointAdr =stIndex;

  ledSetCollor(stIndex,0,0, 255); 
  while(!(analogRead(stIndex)>=analogref))
  {
  }
 }
 void play()
 {
  
 }
  int generatePlayPoint()
 {int stIndex;
 
   stIndex = random(0, 8);


  ledSetCollor(stIndex,255,0, 0);
  return stIndex;
 }
 void allOff()
 {
   ledSetCollor(0,0,0, 0);
   ledSetCollor(1,0,0, 0);
   ledSetCollor(2,0,0, 0);
   ledSetCollor(3,0,0, 0);
   ledSetCollor(4,0,0, 0);
   ledSetCollor(5,0,0, 0);
   ledSetCollor(6,0,0, 0);
   ledSetCollor(7,0,0, 0);
  }
