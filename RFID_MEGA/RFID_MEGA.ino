/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
//Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>

/* printer libs */
//#include "Adafruit_Thermal.h"
//Adafruit_Thermal printer(&Serial3);


#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 3
#define RESET_DIO 5
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 
char uidString[9];
String content = "";
int digitCount = 0;
unsigned long past = 0;
long debounce_val = 50;



/* Define TFT Pins */
//#define LCD_RST 2
//#define LCD_CS A3
//#define LCD_RS A2
//#define LCD_WR A1
//#define LCD_RD A0

//#define LCD_D0 8
//#define LCD_D1 9
//#define LCD_D2 2
//#define LCD_D3 3
//#define LCD_D4 4
//#define LCD_D5 5
//#define LCD_D6 6
//#define LCD_D7 7

uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
uint8_t SwapXY = 0;

uint16_t TS_LEFT = 860;
uint16_t TS_RT  = 180;
uint16_t TS_TOP = 930;
uint16_t TS_BOT = 125;
char *name = "Unknown controller";

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 270 ohms across the X plate
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
#define GREEN   0x0FC0
#define DGRN    0x0AC2
#define LCD_RESET 2

/* Wi-Fi libs */
#include <WiFiEsp.h>                  // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(49,145,139,4);  // IP of the MySQL *server* here
char user[] = "root";              // MySQL user login username
char password[] = "";        // MySQL user login password

// WiFi card example
char ssid[] = "David_WiFi";    // your SSID
char pass[] = "secretpakta";       // your SSID Password

WiFiEspClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);

void setup()
{ 
  Serial.begin(19200);
  /* Enable the SPI interface */

  /* Initialise the RFID reader */


    pinMode(LCD_RESET,OUTPUT);
    digitalWrite(LCD_RESET,HIGH);
    delay(100);
    digitalWrite(LCD_RESET,LOW);//reset the TFT LCD
    delay(200);
    digitalWrite(LCD_RESET,HIGH);

    Serial.println("Initializing TFT...");
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
    tft.setCursor(0,0);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.print("> Booting Up");
    Serial.print("\nInitializing MFRC522...\n");
    SPI.begin(); 
    RC522.init();

    /* printer init */
      pinMode(7, OUTPUT); digitalWrite(7, LOW);

  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
  //mySerial.begin(19200);  // Initialize SoftwareSerial
//  Serial3.begin(9600); // Use this instead if using hardware serial
//  printer.begin();        // Init printer (same regardless of serial type)


  // WIFI
  Serial1.begin(115200);
  WiFi.init(&Serial1);
  tft.setCursor(0,10);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.print("> Connecting to server...");
  delay(2500);
  tft.setCursor(0,20);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.print("> UI loading...");
  delay(1500);
  tft.fillScreen(BLACK);

  
}


void storeHexRepresentation(char *b, const byte v)
{
  if (v <= 0xF) {
    *b = '0';
    b++;
  }
  itoa(v, b, 16); // http://www.cplusplus.com/reference/cstdlib/itoa/
}

String reverseAndConvert(String str) {
  int i;
  unsigned long int value;
  value = 0;
  String temp = "";
  String finalString = "";

  for (i = str.length(); i >= 2; i -= 2)
  {
    temp = temp + str.substring(i - 2, i);
  }
  temp.trim();
  temp = addZeroB(temp);
  //Serial.println("Partial temp: " + temp);
  for (i = 0; i < temp.length(); i++) {
    value = value + (getIntVal(temp.charAt(i)) * dvalOfHex((temp.length() - 1) - i));
    //Serial.println(value);
  }

  finalString.trim();
  finalString = String(value);
  if (finalString.length() < 10) {
    finalString = 0 + finalString;
  }
  return finalString;

}

long int getIntVal(char ch)
{

  switch (ch)
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ch -= 48;
      return ch;
      break;
    case 'a':
      return 10;
      break;
    case 'b':
      return 11;
      break;
    case 'c':
      return 12;
      break;
    case 'd':
      return 13;
      break;
    case 'e':
      return 14;
      break;
    case 'f':
      return 15;
      break;
    default:
      return -1;
  }
}

long int dvalOfHex(int num)
{
  switch (num)
  {
    case 0:
      return 1;
      break;
    case 1:
      return 16;
      break;
    case 2:
      return 256;
      break;
    case 3:
      return 4096;
      break;
    case 4:
      return 65536;
      break;
    case 5:
      return 1048576;
      break;
    case 6:
      return 16777216;
      break;
    case 7:
      return 268435456;
      break;
    default:
      return 0;

  }
}

String addZeroB(String str)
{
  str.trim();
  if (str.length() == 6) {
    str.concat("0b");
  }
  return str;

}

void scanCardRoutine() {
  if ( ! RC522.isCard())
  {
    return;
  }
  // Select one of the cards
  if ( ! RC522.readCardSerial())
  {
    return;
  }
  //Show UID on serial monitor

  byte letter;
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  for (byte i = 0; i < 4; i++)
  {


    content.concat(String(RC522.serNum[i], HEX));
    //storeHexRepresentation(&uidString[2 * i], mfrc522.uid.uidByte[i]);

  }

  content = reverseAndConvert(content);

}


void loop()
{
    int status = WiFi.begin(ssid, pass);
    tft.setCursor(0, 0);
    tft.setTextSize(2.5);
    tft.println("COLEGIO SAN AGUSTIN");
    tft.setCursor(60, 25);
    tft.println("BACOLOD");
    tft.setCursor(50,60);
    tft.setTextSize(2);
    tft.println("Wireless");
    tft.println("     Payment");
    tft.println("     System");
    tft.println(" ");
    tft.setCursor(60,150);
    tft.setTextSize(2);
    tft.println("SCAN CARD");
    tft.setCursor(0,(((tft.height()/9)*8)+15));
    tft.setTextSize(1);
    tft.print("Server Status:");
    if(status == WL_CONNECTED)
    {
      tft.fillRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), GREEN);
      tft.drawRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), GREEN);
      tft.setCursor(((tft.width()/4)+25), (((tft.height()/9)*8)+30));
      tft.setTextSize(2);
      tft.setTextColor(DGRN);
      tft.print("ONLINE");
      while(true);
    }
    else if(status == WL_NO_SHIELD)
    {
      tft.fillRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.drawRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.setCursor(0, (((tft.height()/9)*8)+30));
      tft.setTextSize(2);
      tft.print("WiFi CARD NOT FOUND");
      while(true);
    }
    else if(status == WL_CONNECT_FAILED)
    {
      tft.fillRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.drawRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.setCursor(0,(((tft.height()/9)*8)+30));
      tft.setTextSize(2);
      tft.print("CHECK SSID/PASSKEY");
      while(true);
    }
    else
    {
      tft.fillRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.drawRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.setCursor(((tft.width()/4)+15), (((tft.height()/9)*8)+30));
      tft.setTextSize(2);
      tft.print("OFFLINE");
      while(true);
    }
    
    /* If so then get its serial number */
    scanCardRoutine();
    if(content.length() > 0) {
    Serial.println("Card found: " + content);
    tft.reset();
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.println("Card found:  ");
    tft.println(content);
    tft.fillRoundRect(40, 100, 160, 40, 8, YELLOW);
    tft.drawRoundRect(40, 100, 160, 40, 8, RED); 
    tft.setCursor(105,112);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.println("PAY"); 
    tft.fillRoundRect(40, 150, 160, 40, 8, YELLOW);
    tft.drawRoundRect(40, 150, 160, 40, 8, RED);  
    tft.setCursor(43,163);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.println("CHECK BALANCE"); 
    //printer.printBitmap(csab_logo_small_width, csab_logo_small_height, csab_logo_small_data);
//    printer.justify('C');
//    printer.doubleHeightOn();
//    printer.println("COLEGIO SAN AGUSTIN BACOLOD");
//    printer.doubleHeightOff();
//    printer.println("BS Aquino Drive");
//    printer.println("Bacolod City");
//    printer.println("Card found");
//    printer.println(" ");
//    printer.println(content);
//    char rfidcd[content.length()];
//    content.toCharArray(rfidcd, content.length());
//    printer.setBarcodeHeight(50);
//    printer.printBarcode("123456789123", UPC_A);
//    printer.feed(2);
    delay(20000);
//    printer.sleep();
    uint16_t xpos, ypos;
      tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      pinMode(XP, OUTPUT);
      pinMode(YM, OUTPUT);
      if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        // is controller wired for Landscape ? or are we oriented in Landscape?
        if (SwapXY != (Orientation & 1)) SWAP(tp.x, tp.y);
        // scale from 0->1023 to tft.width  i.e. left = 0, rt = width
        // most mcufriend have touch (with icons) that extends below the TFT
        // screens without icons need to reserve a space for "erase"
        // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
        xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
        ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
      if(xpos > 312 && xpos < 771 && ypos > 560 && ypos < 650) {
    tft.fillScreen(BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.println("CHECK BALANCE");
    }
}
}
}
