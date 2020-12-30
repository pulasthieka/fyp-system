#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int PAT = 0; //for PAT signal
String send_to_server = ""; //what we send to server
String serial = ""; //to store serial buffer
char rx; //to read first byte from serial buffer
uint8_t dum=0;
String payload="";


const char* ssid = "TP-LINK_85D38C";
const char* password = "passcode1";

const char* nodejs_ip = "192.168.1.109";
const int nodejs_port = 90;

WiFiServer server(80);

void setup() {

  Serial.begin(57600);
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

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
    http.begin(cl, "http://" + (String)nodejs_ip + ":90/" + "saveData/" );
    http.addHeader("Content-Type", "application/json");
    Serial.println("Sending data");
    while (1) {
      uint8_t loo;
      for(loo=0;loo<100;loo++){//95
      /////////////////////////////////////////////////Start of main rogram
      //Empty seral buffer
      serial = "";
      while (1) {
        //componsate for data fully not recieved
        while (Serial.available()) {
          rx = Serial.read();
          serial.concat(rx);
        }
        delayMicroseconds(250);
        if (!Serial.available()) {
          break;
        }
      }
      send_to_server.concat(serial);
      //Read PAT signal
      PAT = dum;//analogRead(A0);
      dum--;
      send_to_server.concat("P" + String(PAT));
      //////////////////////////////////////////////End of main program
      delay(9);
      }


      //Serial.println(send_to_server);
      //while(payload!="a"){
      int httpCode = http.POST("{\"hello\":\"" + send_to_server + "\"}");
     // payload = http.getString();
    //  delay(1);
      //Serial.println(send_to_server);
    //  }
      send_to_server="";
    //  payload="";
      //200
    }
    http.end();
  } else {
    Serial.println("connection failed - Nodejs");
  }
}
