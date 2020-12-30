#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

int PAT = 0; //for PAT signal
String send_to_server = ""; //what we send to server
String buffer_Serial2 = "";
String serial = ""; //to store serial buffer
String serial2 = "";
char rx = 'X'; //to read first byte from serial buffer
uint8_t dum = 0;
String payload = "";


const char* ssid = "TP-LINK_85D38C";
const char* password = "passcode1";

const char* nodejs_ip = "192.168.1.101";
const int nodejs_port = 90;

WiFiServer server(80);

void setup() {
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);
  
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
      uint8_t loop_var;
      //Empty seral buffer

      for (loop_var = 0; loop_var < 20; loop_var++) { //95
        /////////////////////////////////////////////////Start of main rogram
        while (digitalRead(22)) {
          delayMicroseconds(1);
        }
        delayMicroseconds(5);

        //componsate for data fully not recieved
        while (1) {
          while (Serial1.available()) { //Reading serial 1
            rx = Serial1.read();
            if (rx != 'X') {
              serial.concat(rx);
            }
          }
          if (rx=='X') {
            break;
          }
          delayMicroseconds(100);
        }

        serial2 = "";
        while (digitalRead(23)) {
          delayMicroseconds(1);
        }
        
          delayMicroseconds(5);
          while (1) {
          while (Serial2.available()) { //Reading serial 2
            rx = Serial2.read();
            Serial.print(rx);
            if (rx != 'X') {
              serial2.concat(rx);
            }
            
           }
          if (rx == 'X') {
            break;
          }
          delayMicroseconds(100);
        }


        buffer_Serial2.concat(serial2);
        //Read PAT signal
        PAT = dum;//analogRead(A0);
        dum--;
        send_to_server.concat("P" + String(PAT));
        //////////////////////////////////////////////End of main program
        // delay(5);
      }
      //send_to_server.concat(serial);
      /////////////////////////Sending data
      Serial.println(serial);
      //Serial.println(send_to_server);
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST("{\"hello\":\"" + send_to_server + "\", \"hello2\":\"" +  buffer_Serial2 + "\" , \"hello3\" : \"" + serial + "\" }");
      //{ "hello": "+send_to_server+", "hello2": "+buffer_Serial2+" }
      //payload = http.getString();
      send_to_server = "";
      buffer_Serial2 = "";
      serial = "";
      //  payload="";

      delay(250);
    }
    http.end();
  } else {
    Serial.println("connection failed - Nodejs");
  }
}
