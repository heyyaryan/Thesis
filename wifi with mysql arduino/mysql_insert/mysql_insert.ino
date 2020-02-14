//#include <WiFi.h> // Use this for WiFi instead of Ethernet.h
#include <WiFiEsp.h> //use for ESP8266
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Emulate Serial1 on pins 2/3 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
//SoftwareSerial Serial1(2, 3); // RX, TX
#endif

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char INSERT_SQL1[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('abcdefg', 355.00)";
char INSERT_SQL2[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('itworks', 6969.00)";
//char INSERT_SQL[] = "SELECT * FROM esp_data.test_tbl";

IPAddress server_addr(192, 168, 200, 102); // IP of the MySQL server here
char user[] = "master";
char password[] = "password";

// WiFi card example
char ssid[] = "Engg_Labs"; // your network SSID (name)
char pass[] = "frankenstein"; // your network password

//WiFiClient client; // Use this for WiFi instead of EthernetClient
WiFiEspClient client; //Use this for ESP8266
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

unsigned long time_now;
int period=2000;

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  // initialize serial for ESP module
  Serial.println("Starting....");
  Serial1.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial1);
  Serial.println("Connecting to WiFi");

  // Begin WiFi section
  int status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }
  // print out info about the connection:
  else {
    Serial.println("Connected to network");
    IPAddress ip = WiFi.localIP();
    Serial.print("My IP address is: ");
    Serial.println(ip);
  }
  // End WiFi section

  //force connection to mysql db
  while (1) {
    Serial.println("Connecting...");
    if (conn.connect(server_addr, 3306, user, password)) {
      break;
    }
    delay(1000);
  }
  

  Serial.println("Connected Successfully");
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);

  time_now = millis();

  //attempt to insert to records into database
  insertRecord(INSERT_SQL1);
  delay(1000);
  insertRecord(INSERT_SQL2);
}

void loop()
{
  
  if(millis() > time_now + period){
    time_now+=period;
  
//  if (conn.connected()) {
//    cursor->execute(INSERT_SQL);
//  }
//  Serial.println("Inserted rows.");
//    insertRecord(INSERT_SQL);
  }

}

void insertRecord(char *sql){
  
    
  if (conn.connected()){
    cursor->execute(sql);
  }
  Serial.println("Record inserted.");
  
  
}

