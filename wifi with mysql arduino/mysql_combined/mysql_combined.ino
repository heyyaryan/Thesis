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

String rfid_result = "";
String bal_result = "";

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



  delay(2000); // wait for a while
  char INSERT_DATA[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('%s',%s)";
  char SELECT_DATA[] = "SELECT data_id,data_money FROM esp_data.test_tbl WHERE data_id='%s'";
  char query[128];
  char money[10];
  float payment = 150.00;
  char rfid_input[] = "h4h4h3h3"; //test values jklmnopq
  char *rfid_res = "";
  char *bal_res = "";
  char *values = "";
  boolean id_exists = false;
  float balance;
  dtostrf(payment, 1, 2, money);
  sprintf(query, INSERT_DATA, rfid_input, money);
  //insertRecord(query);
  delay(1000);
  sprintf(query, SELECT_DATA, rfid_input);

  id_exists = checkID(query, values);


  flushMe();
  if (id_exists) {
    Serial.println("Match found.");

  } else {
    Serial.println("No matches.");
  }


  String temp_data = values;

  int index = 0;
  index = temp_data.indexOf(',');
  rfid_result = temp_data.substring(0, index - 1);
  bal_result = temp_data.substring(index + 1);
  char buff[bal_result.length()];
  bal_result.toCharArray(buff, bal_result.length());
  //transfer bal_result as float
  balance = atof(buff);
  //test if really float
  float diff;
  diff = balance - payment;

  Serial.print("RFID ");
  Serial.println(rfid_result);
  Serial.print("Balance ");
  Serial.println(balance);
  Serial.print("Diff ");
  Serial.println(diff);

}

void loop()
{




}
//user-defined functions
void flushMe() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}


boolean checkID(char *sql, char *result) {
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  //force db connection. is bad? hmm?
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }

  row_values *row = NULL;
  cursor->execute(sql);
  column_names *columns = cursor->get_columns();
  //  do {
  //    row = cursor->get_next_row();
  //    if (row != NULL) {
  //      for (int f = 0; f < columns->num_fields; f++) {
  //        strcat(result, row->values[f]);
  //        if (f < columns->num_fields - 1) {
  //          strcat(result, ",");
  //        }
  //      }
  //
  //      return true;
  //
  //    } else {
  //      return false;
  //    }
  //  } while (row != NULL);
  row = cursor->get_next_row();
  if (row != NULL) {
    strcat(result, row->values[0]);
    strcat(result, ",");
    strcat(result, row->values[1]);
    delete cursor;
    return true;
  } else {
    delete cursor;
    return false;
  }
}


void insertRecord(char *sql) {
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }
  cursor->execute(sql);
  delete cursor;
  Serial.println("Record inserted.");
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


