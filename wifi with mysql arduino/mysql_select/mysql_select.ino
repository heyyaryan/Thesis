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
//char INSERT_SQL1[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('abcdefg', 355.00)";
//char INSERT_SQL2[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('itworks', 6969.00)";

IPAddress server_addr(192, 168, 200, 102); // IP of the MySQL server here
char user[] = "master";
char password[] = "password";

// WiFi card example
char ssid[] = "Engg_Labs"; // your network SSID (name)
char pass[] = "frankenstein"; // your network password

WiFiEspClient client; //Use this for ESP8266
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

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

  //force connection to mysql db: connectDB()
  connectDB();

  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);

  delay(2000); // wait for a while
  char query[] = "SELECT data_id FROM esp_data.test_tbl WHERE data_id='";
  char rfid_input[] = "abcdefk";
  
  strcat(query, rfid_input);
  strcat(query, "'");
  //Serial.println(query);
  
  row_values *row = NULL;
  char *rfid_result = "";
  char *results = "";

  //long result_data=0;
  cursor->execute(query);
  // Fetch the columns (required) but we don't use them.
  column_names *columns = cursor->get_columns();

  // Read the row (we are only expecting the one)
  do {
    row = cursor->get_next_row();
    delay(100);
    if (row != NULL) {
      strcpy(results, row->values[0]);
    }
  } while (row != NULL);

  // Deleting the cursor also frees up memory used
  delete cursor;
  Serial.print("Result data: ");
  Serial.println(results);

}

void loop()
{




}
void checkID(){
  
}

void insertRecord(char *sql) {
  if (conn.connected()) {
    cursor->execute(sql);
  } else {
    conn.close();
    connectDB();
    Serial.println("Record inserted.");
  }
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


