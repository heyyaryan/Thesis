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

/* printer libs */
#include "Adafruit_Thermal.h"
Adafruit_Thermal printer(&Serial3);


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
float amt, newbal, dec, decvw, wh;
float rembal;

//variables used in program
String rfid_result = "";
String bal_result = "";
String transactionDateTime = "";
String transactionID = "";
String temp_data = "";
char query[192];
char money[10];
float payment = 300.00;
char rfid_input[15]; //test values jklmnopq
char *rfid_res = "";
char *bal_res = "";
char *values = "";
int id_exists = 0;
int lastTransID = 0;
float balance;
float difference;
int currentTransID=0;
int index = 0;

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
#define PINK    0xFA9C
#define LCD_RESET 2

/* Wi-Fi libs */
#include <WiFiEsp.h>                  // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(192,168,2,103);  // IP of the MySQL *server* here
char user[] = "mira";              // MySQL user login username
char password[] = "secret";        // MySQL user login password

// WiFi card example
char ssid[] = "RFID_Wifi";    // your SSID
char pass[] = "password";       // your SSID Password

WiFiEspClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

void setup()
{ 
  Serial.begin(19200);
  /* Enable the SPI interface */

  /* Initialise the RFID reader */

    Serial.println("Initializing TFT...");
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
    tft.setCursor(0,0);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.print("> Booting Up");
    Serial.print("\nInitializing MFRC522...\n");
    SPI.begin(); 
    RC522.init();

    /* printer init */
  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
  //mySerial.begin(19200);  // Initialize SoftwareSerial
  Serial3.begin(9600); // Use this instead if using hardware serial
  printer.begin();        // Init printer (same regardless of serial type)


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

void connectDB() {
  while (1) {
    Serial.println("Connecting...");
    if (conn.connect(server_addr, 3306, user, password)) {
      break;
    }
    delay(1000);
  }
  //can be commented out
  Serial.println("Connected Successfully");
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

void deci()
{
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  while (true)
  {
  uint16_t xpos, ypos;
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);

  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
  {
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    
    Serial.println("\nXPOS = ");
    Serial.print(xpos);
    Serial.println("\nYPOS = ");
    Serial.print(ypos);

          //enter command
    if(xpos>200 && xpos<250 && ypos>260 && ypos<400)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if(rembal >= amt)
      {
      newbal = rembal - amt;
      tft.fillScreen(BLACK);
      confirmPayment();
      }
      else
      {
        tft.fillRect(0, 83, tft.width(), 30, BLACK);
        tft.drawRect(0, 83, tft.width(), 30, BLACK);
        tft.setCursor(0,95);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.print("INSUFFICIENT BALANCE");
        delay(3000L);
        kp();
      }
    }

        //clear
    if(xpos>200 && xpos<250 && ypos>120 && ypos<260)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,85);
      tft.setTextColor(BLACK);
      tft.setTextSize(4);
      tft.drawRect(55,85,tft.width(),40,BLACK);
      tft.fillRect(55,85,tft.width(),40,BLACK);
      amt = 0;
      wh = 0;
      dec = 0;
      kp();
      Serial.println(amt, 2);
    }

        //0
    if(xpos>0 && xpos<120 && ypos>330 && ypos<410)
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      tft.setCursor(60,85);
      tft.setTextColor(WHITE);
      tft.setTextSize(4);
      if(dec == 0)
      {
        dec = 0.00;
      }
      else
      {
        dec = (dec/0.1) + 0.00;
      }
      amt = wh + dec;
      tft.print(amt, 0);
      Serial.println(amt, 0);
    }
    
          //1
      if(xpos>0 && xpos<50 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.01;
        }
        else
        {
          dec = (dec/0.1) + 0.01;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //2
      if(xpos>51 && xpos<120 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.02;
        }
        else
        {
          dec = (dec/0.1) + 0.02;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //3
      if(xpos>121 && xpos<180 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.03;
        }
        else
        {
          dec = (dec/0.1) + 0.03;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //4
      if(xpos>0 && xpos<50 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.04;
        }
        else
        {
          dec = (dec/0.1) + 0.04;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //5
      if(xpos>51 && xpos<120 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.05;
        }
        else
        {
          dec = (dec/0.1) + 0.05;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //6
      if(xpos>121 && xpos<180 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.06;
        }
        else
        {
          dec = (dec/0.1) + 0.06;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
          //7
      if(xpos>0 && xpos<50 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.07;
        }
        else
        {
          dec = (dec/0.1) + 0.07;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //8
      if(xpos>51 && xpos<120 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.08;
        }
        else
        {
          dec = (dec/0.1) + 0.08;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //9
      if(xpos>121 && xpos<180 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(dec == 0)
        {
          dec = 0.09;
        }
        else
        {
          dec = (dec/0.1) + 0.09;
        }
        amt = wh + dec;
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        tft.print(amt);
        Serial.println(amt, 2);
      }
    delay(500);
    }
  }
}

void confirmPayment()
{
    tft.fillScreen(BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("CONFIRM PAYMENT");
    tft.print("\n\nAmount: P ");
    tft.print(amt);
    tft.print("\nCard No.: " + content);
    tft.print("\nNew Bal.: P ");
    tft.print(newbal);

    tft.fillRect(((tft.width()/4)*1), ((tft.height()/8)*2), ((tft.width()/4)*2), 90, BLACK); 
    tft.drawRect(((tft.width()/4)*1), ((tft.height()/8)*2), ((tft.width()/4)*2), 90, PINK); 
    tft.setCursor(((tft.width()/4)*1)+18, ((tft.height()/8)*2)+38);
    tft.setTextSize(2);
    tft.setTextColor(PINK);
    tft.print("CONFIRM");

    tft.fillRect(((tft.width()/4)*1), (((tft.height()/8)*3)+60), ((tft.width()/4)*2), 90, BLACK); 
    tft.drawRect(((tft.width()/4)*1), (((tft.height()/8)*3)+60), ((tft.width()/4)*2), 90, RED); 
    tft.setCursor(((tft.width()/4)*1)+25, (((tft.height()/8)*3)+93));
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.print("CANCEL");

    while(true)
    {
      uint16_t xpos, ypos;
      tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      pinMode(XP, OUTPUT);
      pinMode(YM, OUTPUT);

      if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
      {
        xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
        ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
    
        Serial.println("\nXPOS = ");
        Serial.print(xpos);
        Serial.println("\nYPOS = ");
        Serial.print(ypos);
    
        if(xpos>50 && xpos<180 && ypos>60 && ypos<177)
        {
          tft.fillScreen(BLACK);
          tft.setCursor(20,10);
          tft.setTextSize(3);
          tft.print("Payment");
          tft.setCursor(20,40);
          tft.setTextSize(3);
          tft.print("Success!");
          tft.setCursor(15,70);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.print("Print Receipt?");
          
        tft.fillRect(((tft.width()/4)*1), ((tft.height()/8)*2), ((tft.width()/4)*2), 90, BLACK); 
        tft.drawRect(((tft.width()/4)*1), ((tft.height()/8)*2), ((tft.width()/4)*2), 90, PINK); 
        tft.setCursor(((tft.width()/4)*1)+18, ((tft.height()/8)*2)+38);
        tft.setTextSize(3);
        tft.setTextColor(PINK);
        tft.print("YES");
    
        tft.fillRect(((tft.width()/4)*1), (((tft.height()/8)*3)+60), ((tft.width()/4)*2), 90, BLACK); 
        tft.drawRect(((tft.width()/4)*1), (((tft.height()/8)*3)+60), ((tft.width()/4)*2), 90, RED); 
        tft.setCursor(((tft.width()/4)*1)+25, (((tft.height()/8)*3)+93));
        tft.setTextSize(3);
        tft.setTextColor(RED);
        tft.print("NO");
    
        printtrans();
    
    
        }
    
        if(xpos>50 && xpos<180 && ypos>200 && ypos<300)
        {
          tft.fillScreen(BLACK);
          tft.setCursor(0,0);
          tft.setTextSize(2);
          tft.print("Payment Cancelled");
          amt = 0;
          wh = 0;
          dec = 0;
          rembal = 0;
          newbal = 0;
          content = "";
          delay(1500);
          loop();
        }
      }
    }
}

int checkID(char *id) {
  char CHECK_USER[] = "SELECT balance FROM rfidcard_db.user_data WHERE rfid_num='%s'";
  sprintf(query, CHECK_USER, id);
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  //force db connection. is bad? hmm?
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }
  row_values *row = NULL;
  cursor->execute(query);
  column_names *columns = cursor->get_columns();

  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      strcpy(bal_res, row->values[0]);
      id_exists = 1;
      break;
    }else{
      id_exists = 0;
      break;
    }
  } while (row != NULL);
 
  delete cursor;
  //clear query
  memset(query,0,sizeof(query));
  return id_exists;
  //conn.close();
  delay(1000);
}

int checkTransID(){
  int result = 0;
  char CHECK_LAST_TRAN[] = "SELECT transactionID FROM rfidcard_db.transaction_data ORDER BY transactionID DESC LIMIT 1";
  cursor = new MySQL_Cursor(&conn);
  //force db connection. is bad? hmm?
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }
  row_values *row = NULL;
  cursor->execute(CHECK_LAST_TRAN);
  column_names *columns = cursor->get_columns();

  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      result = atoi(row->values[0]);
    }
  
  } while (row != NULL);
 
  delete cursor;
  //clear query
  return result;
  //conn.close();
  delay(1000);
}

void kp()
{
    currentTransID = checkTransID() + 1;
    Serial.print("Current TransID ");
    Serial.println(currentTransID);

    checkID(rfid_input);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("Card No.: " + content);
    tft.print("\nRem. Bal.: ");
    //transfer bal_result as float
    rembal = atof(bal_res);
    tft.print(rembal);
    tft.setCursor(0,50);
    tft.print("Enter Amount:");
    tft.setCursor(0,85);
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

    while (true)
    {
    uint16_t xpos, ypos;
    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);
  
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
      xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
      ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
  
      
      Serial.println("\nXPOS = ");
      Serial.print(xpos);
      Serial.println("\nYPOS = ");
      Serial.print(ypos);
  
      //enter command
      if(xpos>200 && xpos<250 && ypos>260 && ypos<410)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if(rembal >= amt)
        {
        newbal = rembal - amt;
        tft.fillScreen(BLACK);
        confirmPayment();
        }
        
        else
        {
          tft.fillRect(0, 83, tft.width(), 30, BLACK);
          tft.drawRect(0, 83, tft.width(), 30, BLACK);
          tft.setCursor(0,95);
          tft.setTextColor(RED);
          tft.setTextSize(2);
          tft.print("INSUFFICIENT BALANCE");
          amt = 0;
          dec = 0;
          wh = 0;
          delay(3000L);
          kp();
        }
      }
  
      //clear
      if(xpos>200 && xpos<250 && ypos>120 && ypos<260)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(BLACK);
        tft.setTextSize(4);
        tft.drawRect(55,85,tft.width(),40,BLACK);
        tft.fillRect(55,85,tft.width(),40,BLACK);
        amt = 0;
        wh = 0;
        dec = 0;
        kp();
        tft.print(amt);
        Serial.println(amt, 2);
      }
  
      //0
      if(xpos>0 && xpos<120 && ypos>330 && ypos<410)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 0;
        }
        else
        {
          wh = (wh*10) + 0;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //1
      if(xpos>0 && xpos<50 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 1;
        }
        else
        {
          wh = (wh*10) + 1;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //2
      if(xpos>51 && xpos<120 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 2;
        }
        else
        {
          wh = (wh*10) + 2;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //3
      if(xpos>121 && xpos<180 && ypos>260 && ypos<330)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 3;
        }
        else
        {
          wh = (wh*10) + 3;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //4
      if(xpos>0 && xpos<50 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 4;
        }
        else
        {
          wh = (wh*10) + 4;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //5
      if(xpos>51 && xpos<120 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 5;
        }
        else
        {
          wh = (wh*10) + 5;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //6
      if(xpos>121 && xpos<180 && ypos>185 && ypos<259)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 6;
        }
        else
        {
          wh = (wh*10) + 6;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
          //7
      if(xpos>0 && xpos<50 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 7;
        }
        else
        {
          wh = (wh*10) + 7;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //8
      if(xpos>51 && xpos<120 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 8;
        }
        else
        {
          wh = (wh*10) + 8;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
      //9
      if(xpos>121 && xpos<180 && ypos>120 && ypos<185)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.setCursor(60,85);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        if(wh == 0)
        {
          wh = 9;
        }
        else
        {
          wh = (wh*10) + 9;
        }
        amt = wh + dec;
        tft.print(amt, 0);
        Serial.println(amt, 0);
      }
  
          //.
      if(xpos>120 && xpos<180 && ypos>330 && ypos<400)
      {
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        deci();
      }
      
      delay(500);
    }
  }
}

void printtrans()
{
while(true)
  {
    uint16_t xpos, ypos;
    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);

    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
      xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
      ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
    
      Serial.println("\nXPOS = ");
      Serial.print(xpos);
      Serial.println("\nYPOS = ");
      Serial.print(ypos);

      if(xpos>50 && xpos<180 && ypos>60 && ypos<177)
      {
        tft.fillScreen(BLACK);
        tft.setCursor(10,10);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("Printing Receipt...");
        printer.justify('C');
        printer.doubleHeightOn();
        printer.println("COLEGIO SAN AGUSTIN BACOLOD");
        printer.doubleHeightOff();
        printer.justify('C');
        printer.println("BS Aquino Drive");
        printer.println("Bacolod City");
        printer.feed(1);
        printer.inverseOn();
        printer.justify('C');
        printer.println("TRANSACTION RECEIPT");
        printer.inverseOff();
        printer.justify('L');
        printer.feed(1);
        printer.println("TranCode: 5857854");
        printer.println("Vendor: BOOKSTORE");
        printer.println("Card No: " + content);
        printer.println("Cardholder: DAVID, RJ");
        printer.println("Dept: COE");
        printer.doubleHeightOn();
        printer.print("Amount: ");
        printer.println(amt);
        printer.doubleHeightOff();
        printer.print("New Bal: ");
        printer.println(newbal);
        printer.feed(1);
        printer.println("Thank You! Come Again");
        printer.println("---------------------------------");
        printer.feed(1);
        printer.sleep();
        tft.fillScreen(BLACK);
        tft.setCursor(10,10);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("You may now \n tear the receipt.");
        amt = 0;
        dec = 0;
        wh = 0;
        rembal = 0;
        newbal = 0;
        content = "";
        Serial.println(content);
        delay(1000L);
        loop();
      }

      if(xpos>50 && xpos<180 && ypos>200 && ypos<300)
      {
        tft.fillScreen(BLACK);
        tft.setCursor(10,10);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.print("Thank you.");
        amt = 0;
        dec = 0;
        wh = 0;
        rembal = 0;
        newbal = 0;
        content = "";
        Serial.println(content);
        delay(1000L);
        loop();
      }
    }
  }
}

void homeScreen()
{
    int status = WiFi.begin(ssid, pass);
    tft.fillScreen(BLACK);
    tft.setCursor(5, 0);
    tft.setTextSize(2.5);
    tft.setTextColor(WHITE);
    tft.println("COLEGIO SAN AGUSTIN");
    tft.setCursor(60, 25);
    tft.println("BACOLOD");
    tft.setCursor(50,60);
    tft.setTextSize(2);
    tft.println(" Wireless");
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
    }
    else
    {
      tft.fillRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.drawRect(0, (((tft.height()/9)*8)+25), tft.width(), tft.height(), RED);
      tft.setCursor(((tft.width()/4)+15), (((tft.height()/9)*8)+30));
      tft.setTextSize(2);
      tft.print("OFFLINE");
    }
}


void loop()
{
  int status = WiFi.begin(ssid, pass);
  homeScreen();
  connectDB();
  while(true)
  {
    scanCardRoutine();
    /* If so then get its serial number */
    if(content.length() > 0) 
    {
    Serial.println("Card found: " + content);
      if(status == WL_CONNECTED)
      {
        content.toCharArray(rfid_input,sizeof(rfid_input));
        checkID(rfid_input);
        if(id_exists)
        {
          kp();
        }
        else
        {
          tft.setCursor(20,250);
          tft.setTextSize(2);
          tft.setTextColor(RED);
          tft.print("CARD NOT FOUND");
          amt = 0;
          dec = 0;
          wh = 0;
          rembal = 0;
          newbal = 0;
          content = "";
          delay(1500);
          loop();
          break;
        }
      }
      else
      {
        tft.setCursor(20,250);
        tft.setTextSize(2);
        tft.setTextColor(RED);
        tft.print("CHECK CONNECTION");
        amt = 0;
        dec = 0;
        wh = 0;
        rembal = 0;
        newbal = 0;
        content = "";
        break;
      }
    }
  }
}
