#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
//Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
uint8_t SwapXY = 0;

uint16_t TS_LEFT = 860;
uint16_t TS_RT = 180;
uint16_t TS_TOP = 930;
uint16_t TS_BOT = 125;
char *name = "Unknown controller";

boolean butEn = true, deci = false;

float num, amt, dec;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 270);
TSPoint tp;

#define MINPRESSURE 20
#define MAXPRESSURE 1000

#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor;
uint8_t Orientation = 0;    //PORTRAIT

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PINK    0xFA9C
#define LCD_RESET 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    pinMode(LCD_RESET,OUTPUT);
    digitalWrite(LCD_RESET,HIGH);
    delay(100);
    digitalWrite(LCD_RESET,LOW);//reset the TFT LCD
    delay(200);
    digitalWrite(LCD_RESET,HIGH);

uint16_t tmp;
    tft.begin(9600);
    
    tft.reset();
    identifier = tft.readID();
  
    switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    }

    tft.begin(identifier);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("Enter Amount:");
    tft.setCursor(0,50);
    tft.setTextSize(4);
    tft.print("P");

    // 0
    tft.fillRect(0,((tft.height()/6)*5),((tft.width()/4)*2),(tft.height()/6), BLACK);
    tft.drawRect(0,((tft.height()/6)*5),((tft.width()/4)*2),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*1)+30),(((tft.height()/6)*5)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("0");

    // .
    tft.fillRect(((tft.width()/4)*2),((tft.height()/6)*5),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*2),((tft.height()/6)*5),((tft.width()/4)),(tft.height()/6), PINK);\
    tft.setCursor((((tft.width()/4)*2)+17),(((tft.height()/6)*5)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print(".");

    // CLEAR
    tft.fillRect(((tft.width()/4)*3),((tft.height()/6)*4),((tft.width()/4)),((tft.height()/6)*2), BLACK);
    tft.drawRect(((tft.width()/4)*3),((tft.height()/6)*4),((tft.width()/4)),((tft.height()/6)*2), PINK);
    tft.setCursor((((tft.width()/4)*3)),(((tft.height()/6)*4)+60));
    tft.setTextSize(2);
    tft.setTextColor(PINK);
    tft.print("ENTER");

    // 1
    tft.fillRect(0,((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(0,((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor(17,(((tft.height()/6)*4)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("1");

    // 2
    tft.fillRect(((tft.width()/4)*1),((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*1),((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*1)+20),(((tft.height()/6)*4)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("2");

    // 3
    tft.fillRect(((tft.width()/4)*2),((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*2),((tft.height()/6)*4),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*2)+20),(((tft.height()/6)*4)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("3");
    
    // 4
    tft.fillRect(0,((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(0,((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor(17,(((tft.height()/6)*3)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("4");

    // 5
    tft.fillRect(((tft.width()/4)*1),((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*1),((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*1)+20),(((tft.height()/6)*3)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("5");

    // 6
    tft.fillRect(((tft.width()/4)*2),((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*2),((tft.height()/6)*3),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*2)+20),(((tft.height()/6)*3)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("6");

    // 7
    tft.fillRect(0,((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(0,((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor(17,(((tft.height()/6)*2)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("7");


    // 8
    tft.fillRect(((tft.width()/4)*1),((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*1),((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*1)+20),(((tft.height()/6)*2)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("8");

    // 9
    tft.fillRect(((tft.width()/4)*2),((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), BLACK);
    tft.drawRect(((tft.width()/4)*2),((tft.height()/6)*2),((tft.width()/4)),(tft.height()/6), PINK);
    tft.setCursor((((tft.width()/4)*2)+20),(((tft.height()/6)*2)+20));
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print("9");

    // ENTER
    tft.fillRect(((tft.width()/4)*3),((tft.height()/6)*2),((tft.width()/4)),((tft.height()/6)*2), BLACK);
    tft.drawRect(((tft.width()/4)*3),((tft.height()/6)*2),((tft.width()/4)),((tft.height()/6)*2), PINK);
    tft.setCursor((((tft.width()/4)*3)),(((tft.height()/6)*2)+60));
    tft.setTextSize(2);
    tft.setTextColor(PINK);
    tft.print("CLEAR");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t xpos, ypos;
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);

//  Serial.println("P = ");
//  Serial.print(tp);
//  Serial.println("P.X = ");
//  Serial.print(tp.x);
//  Serial.println("P.Y = ");
//  Serial.print(tp.y);
//  Serial.println("P.z = ");
//  Serial.print(tp.z);

  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
  {
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    
    Serial.println("\nXPOS = ");
    Serial.print(xpos);
    Serial.println("\nYPOS = ");
    Serial.print(ypos);

    //enter command
    if(xpos>200 && xpos<250 && ypos>270 && ypos<410)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.fillScreen(BLACK);
      tft.setCursor(50,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("Entered Amt");
      tft.setCursor(50,60);
      tft.setTextSize(4);
      tft.println(amt);
    }

    //clear
    if(xpos>180 && xpos<200 && ypos>120 && ypos<260)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(BLACK);
      tft.setTextSize(4);
      tft.print(amt, 2);
      amt = 0;
      Serial.println(amt, 2);
    }

    //0
    if(xpos>0 && xpos<120 && ypos>330 && ypos<410)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 0;
      }
      else
      {
        amt = (amt*10) + 0;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //1
    if(xpos>0 && xpos<50 && ypos>260 && ypos<330)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 1;
      }
      else
      {
        amt = (amt*10) + 1;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //2
    if(xpos>51 && xpos<120 && ypos>260 && ypos<330)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 2;
      }
      else
      {
        amt = (amt*10) + 2;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //3
    if(xpos>121 && xpos<180 && ypos>260 && ypos<330)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 3;
      }
      else
      {
        amt = (amt*10) + 3;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //4
    if(xpos>0 && xpos<50 && ypos>185 && ypos<259)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 4;
      }
      else
      {
        amt = (amt*10) + 4;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //5
    if(xpos>51 && xpos<120 && ypos>185 && ypos<259)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 5;
      }
      else
      {
        amt = (amt*10) + 5;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //6
    if(xpos>121 && xpos<180 && ypos>185 && ypos<259)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 6;
      }
      else
      {
        amt = (amt*10) + 6;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

        //7
    if(xpos>0 && xpos<50 && ypos>120 && ypos<185)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 7;
      }
      else
      {
        amt = (amt*10) + 7;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //8
    if(xpos>51 && xpos<120 && ypos>120 && ypos<185)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 8;
      }
      else
      {
        amt = (amt*10) + 8;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

    //9
    if(xpos>121 && xpos<180 && ypos>120 && ypos<185)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(amt == 0)
      {
        amt = 9;
      }
      else
      {
        amt = (amt*10) + 9;
      }
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }

        //.
    if(xpos>120 && xpos<180 && ypos>330 && ypos<400)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(175,50);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      deci = true;

      while(deci == true)
      {
          //1
      if(xpos>0 && xpos<50 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.01;
        }
        else
        {
          amt = (amt*0.1) + 0.01;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //2
      if(xpos>51 && xpos<120 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.02;
        }
        else
        {
          amt = (amt*0.1) + 0.02;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //3
      if(xpos>121 && xpos<180 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.03;
        }
        else
        {
          amt = (amt*0.10) + 0.03;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //4
      if(xpos>0 && xpos<50 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.04;
        }
        else
        {
          amt = (amt*0.10) + 0.04;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //5
      if(xpos>51 && xpos<120 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.05;
        }
        else
        {
          amt = (amt*0.10) + 0.05;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //6
      if(xpos>121 && xpos<180 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.06;
        }
        else
        {
          amt = (amt*0.10) + 0.06;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
          //7
      if(xpos>0 && xpos<50 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.07;
        }
        else
        {
          amt = (amt*0.10) + 0.07;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //8
      if(xpos>51 && xpos<120 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.08;
        }
        else
        {
          amt = (amt*0.10) + 0.08;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
  
      //9
      if(xpos>121 && xpos<180 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,50);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(amt == 0)
        {
          amt = 0.09;
        }
        else
        {
          amt = (amt*0.10) + 0.09;
        }
        tft.print(amt, 2);
        Serial.println(amt, 2);
      }
    }
    }
    
    delay(500);
  }
}
