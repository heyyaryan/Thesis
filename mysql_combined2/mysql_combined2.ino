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


IPAddress server_addr(192, 168, 2, 103); // IP of the MySQL server here
char user[] = "mira";
char password[] = "secret";

// WiFi card example
char ssid[] = "RFID_Wifi"; // your network SSID (name)
char pass[] = "password"; // your network password

WiFiEspClient client; //Use this for ESP8266
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

//variables used in program
String rfid_result = "";
String bal_result = "";
String transactionDateTime = "";
String transactionID = "";
String temp_data = "";
char query[192];
char money[10];
float payment = 300.00;
char rfid_input[] = "testmira"; //test values jklmnopq
char rfid_input2[] = "12abcd34";
char *rfid_res = "";
char *bal_res = "";
char *values = "";
int id_exists = 0;
int lastTransID = 0;
float balance;
float difference;
int currentTransID=0;

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
  delay(1000);


  //SQL STATEMENTS and TEST DATA
  //char INSERT_DATA[] = "INSERT INTO esp_data.test_tbl (data_id, data_money) VALUES ('%s',%s)";
  //char INSERT_DATA[] = "INSERT INTO rfidcard_db.transaction_data (transactionID,rfid_num,transactionAmount,transactionType) VALUES (69,'%s',%s,'AM')";
  //char SEL_DATA_TRAN[] = "SELECT transactionID,transactionDateTime,rfid_num,transactionAmount FROM esp_data.transaction_data WHERE rfid_num='%s'";
  //char CHECK_LAST_TRAN[] = "SELECT transactionID FROM rfidcard_db.transaction_data ORDER BY transactionID DESC LIMIT 1";
  
  currentTransID = checkTransID() + 1;
  Serial.print("Current TransID ");
  Serial.println(currentTransID);
  
  
  checkID(rfid_input2);
  
  if (id_exists){
    Serial.println("Match found.");
  } else {
    Serial.println("No matches.");
  }
  Serial.print("RFID ");
  Serial.println(rfid_input2);
  Serial.print("Balance ");
  Serial.println(bal_res);
 
  memset(bal_res,0,sizeof(bal_res));
  
  checkID(rfid_input);
  if (id_exists){
    Serial.println("Match found.");
  } else {
    Serial.println("No matches.");
  }
  Serial.print("RFID ");
  Serial.println(rfid_input2);
  Serial.print("Balance ");
  Serial.println(bal_res);
  
  
  
  
  /*DONT DELETE. RESERVED FOR CONVERTING STRING / CHAR TO FLOAT*/
  //  temp_data=values;

  //    int index = 0;
  //    index = temp_data.indexOf(',');
  //    rfid_result = temp_data.substring(0, index - 1);
  //    bal_result = temp_data.substring(index + 1);
  //    bal_result=temp_data;
  //    char buff[bal_result.length()];
  //    bal_result.toCharArray(buff, bal_result.length());
  //    //transfer bal_result as float
  //    balance = atof(buff);
  //    //test if really float
  //    diff = balance - payment;
  /*************************************************************/
  

  Serial.print(" RAM: ");
  Serial.println(get_free_memory());


  //conn.close();
}

void loop()
{




}

void flushMe() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
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

void insertTransaction(int trans_id, char *rfid) {
  //prepare statement
  char INSERT_DATA[] = "INSERT INTO rfidcard_db.transaction_data (transactionID,rfid_num,transactionAmount,transactionType) VALUES (%d,'%s',%s,'PR')";
  dtostrf(payment, 1, 2, money);
  //Serial.println(trans_id);
  sprintf(query, INSERT_DATA, trans_id, rfid, money);
  //Serial.println(query);
  //execute query
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }
  cursor->execute(query);
  delete cursor;
  Serial.println("Record inserted.");
  //clear queries and money array;
  memset(query, 0, sizeof(query));
  memset(money, 0, sizeof(money));
  //verify if cleared
  //  Serial.print("query ");
  //  Serial.println(query);
  //  Serial.print("money ");
  //  Serial.println(money);
  delay(1000);
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

int get_free_memory()
{
  extern char __bss_end;
  extern char *__brkval;
  int free_memory;
  if ((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);
  return free_memory;
}
void getDateTime(char *result) {
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  //force db connection. is bad? hmm?
  while (!conn.connected()) {
    conn.close();
    connectDB();
  }

  row_values *row = NULL;
  cursor->execute("SELECT NOW()");
  column_names *columns = cursor->get_columns();
  row = cursor->get_next_row();
  if (row != NULL) {
    strcat(result, row->values[0]);
    delete cursor;

  }
}

void selectPrintData(char *sql, char *result) {
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
  row = cursor->get_next_row();
  if (row != NULL) {
    strcat(result, row->values[0]);
    strcat(result, ",");
    strcat(result, row->values[1]);
    strcat(result, ",");
    strcat(result, row->values[2]);
    strcat(result, ",");
    strcat(result, row->values[3]);

    delete cursor;

  }
}

void idDataSplit(char *values) {


}
//user-defined functions






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


