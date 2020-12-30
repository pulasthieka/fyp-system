int ecg[197]={314,313,315,314,314,316,318,316,315,317,319,318,317,319,320,319,319,320,320,319,319,322,322,322,322,325,326,325,324,325,324,324,329,336,339,340,339,337,333,331,330,327,323,321,324,324,323,322,323,323,322,323,325,325,323,324,326,332,352,401,493,578,612,532,391,261,212,231,267,295,313,327,330,330,331,334,336,336,339,342,342,343,346,349,349,349,352,355,355,356,361,363,364,365,370,374,376,379,385,389,393,398,404,409,413,418,423,424,423,423,420,414,405,395,381,364,347,335,323,311,302,298,293,289,287,287,285,283,284,286,286,285,286,288,287,287,290,292,292,292,296,298,297,298,301,301,301,301,303,303,303,303,305,304,304,306,308,307,305,306,307,306,306,307,308,307,306,308,309,309,308,310,309,308,309,311,311,310,311,313,312,311,314,316,315,314,316,317,315,314,316,317,317,317,315,314,314};
#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 250;  
int PAT = 0; //for PAT signal
String P_data = "["; //to store ADC (P data)
//String buffer_Serial2 = "";
String E_data = "["; //to store serial buffer of Serial 1 [E]
String T_data = "["; //to store serial buffer of Serial 1 [T]
String S_data = "["; //to store serial buffer of Serial 2 [S]
String B_data = "["; //to store serial buffer of Serial 2 [B]
bool E_active = 0; //Lossless communication variables
bool T_active = 0;
bool S_active = 0;
bool B_active = 0;
//String serial2 = "";
String xxx;
char rx = 'X'; //to read first byte from serial buffer
uint8_t dum = 0;
String payload = "";


const char* ssid = "TP-LINK_85D38C";
const char* password = "passcode1";

const char* nodejs_ip = "192.168.1.100";
const int nodejs_port = 90;

WiFiServer server(80);

void setup() {
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  Serial.begin(2000000);
  Serial1.begin(2000000);
  Serial2.begin(2000000);

  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  Serial1.setRxBufferSize(1024);
  Serial2.setRxBufferSize(1024);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  Send_data(client);
  delay(1);
}


void Send_data(WiFiClient cl) {
  if (cl.connect(nodejs_ip, nodejs_port)) {
    Serial.println("connected to Nodejs");
    HTTPClient http;
    http.begin("http://" + (String)nodejs_ip + ":90/" + "saveData/" );
    http.addHeader("Content-Type", "application/json");
    Serial.println("Sending data");

    while (1) {
      
      currentMillis = millis();
      while (currentMillis - previousMillis < interval) {
        currentMillis = millis();
        delayMicroseconds(1);
        /////////////////
        delayMicroseconds(5);

        //componsate for data fully not recieved
        while (1) {

          while (Serial1.available()) { //Reading serial 1
            rx = Serial1.read();
            //            Serial.print(rx);
            if (rx != 'X') {
              if ((T_active) and (rx != 'T') and (rx != 'E')) { //asci number //switch
                T_data = T_data + rx ;
              }
              else if ((E_active) and (rx != 'T') and (rx != 'E')) {
                E_data = E_data + rx ;
              }
              else if (rx == 'T') {
                T_active = 1;
              }
              else if (rx == 'E') {
                E_active = 1;
              }
            }
            else { //currently active apend
              if ((T_active) and (T_data[T_data.length() - 1] != ',')) {
                T_data = T_data + ',';
              }
              if ((E_active) and (E_data[E_data.length() - 1] != ',') ) {
                E_data = E_data + ',';
              }
              T_active = 0;
              E_active = 0;
            }

          }
          if (rx == 'X') {
            T_active = 0;
            E_active = 0;
            break;
          }
          delayMicroseconds(10);
        }

     //   serial2 = "";
        
        //componsate for data fully not recieved
        while (1) {

          while (Serial2.available()) { //Reading serial 2 S==I
            rx = Serial2.read();
            if (rx != 'X') {
              if (  (S_active) and (rx != 'I') and (rx != 'B') ) {
                S_data = S_data + rx ;
              }
              else if ( (B_active) and (rx != 'I') and (rx != 'B')  ) {
                B_data = B_data + rx ;
              }
              else if (rx == 'I') {
                S_active = 1;
              }
              else if (rx == 'B') {
                B_active = 1;
              }
            }
            else {
              if ((S_active) and (S_data[S_data.length() - 1] != ',') ) {
                S_data = S_data + ',';
              }
              if ((B_active) and (B_data[B_data.length() - 1] != ',')) {
                B_data = B_data + ',';
              }
              S_active = 0;
              B_active = 0;
            }

          }
          if (rx == 'X') {
            S_active = 0;
            B_active = 0;
            break;
          }
          delayMicroseconds(10);
        }


       // buffer_Serial2.concat(serial2);


        /////////////////     
      
      }
      previousMillis = currentMillis;
      
      uint8_t loop_var;
      //for (loop_var = 0; loop_var < 20; loop_var++) { //95
        /////////////////////////////////////////////////Start of main rogram

        
      for (loop_var = 0; loop_var < 20; loop_var++) { 
        //Read PAT signal
        PAT = ecg[dum];//analogRead(A0);
        dum++;
        P_data.concat(String(PAT) + ",");
        //////////////////////////////////////////////End of main program
        if(dum==196){
        dum=0;
        }
      }
      PAT = ecg[dum];//analogRead(A0);
      dum++;
      P_data.concat(String(PAT) + "]"); //Completing list
      if(dum==196){
        dum=0;
        }
      //P_data.concat(serial);
      /////////////////////////Sending data

      //Serial.println(P_data);
      //check it last char is , first
      if (T_data[T_data.length() - 1] == ',') {
        T_data.remove(T_data.length() - 1);  //removing ending coma
      }
      if (E_data[E_data.length() - 1] == ',') {
        E_data.remove(E_data.length() - 1);
      }
      if (S_data[S_data.length() - 1] == ',') {
        S_data.remove(S_data.length() - 1);
      }
      if (B_data[B_data.length() - 1] == ',') {
        B_data.remove(B_data.length() - 1);
      }
      http.addHeader("Content-Type", "application/json");
      xxx = "{\"hello\":" + P_data + ", \"S_data\":" + S_data + "] , \"B_data\" : " + B_data + "], \"T_data\" : " + T_data + "],\"E_data\" : " + E_data + "] }";
      //      Serial.println("");
      //      Serial.println(T_data);
      //      Serial.println(E_data);
      //      Serial.println("");
     // Serial.println("will send");
     // Serial.println(xxx);
      int httpCode = http.POST(xxx);
      //payload = http.getString();
      P_data = "[";
//      buffer_Serial2 = "";
      T_data = "[";
      E_data = "[";
      S_data = "[";
      B_data = "[";

      //delay(250);
    }
    http.end();
  } else {
    Serial.println("connection failed - Nodejs");
  }
}
