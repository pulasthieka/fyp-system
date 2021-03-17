#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>

#include <HardwareSerial.h>
HardwareSerial MySerial1(1);

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

bool wsconnected = false;
unsigned long timeStamps;
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 100;

// DATA Vars
String TData = "[";
String EData = "[";
String BData = "[";
String SData = "[";
String PData = "[";
String TCData = "[";
String ECData = "[";
String BCData = "[";
String SCData = "[";
String PCData = "[";
String P_data = "["; //to store ADC (P data) not a buffer
long PAT = 0;        //for PAT signal

//time Vars
String Ttime = "[";
String Etime = "[";
String Btime = "[";
String Stime = "[";
String Ptime = "[";
String TCtime = "[";
String ECtime = "[";
String BCtime = "[";
String SCtime = "[";
String PCtime = "[";
String payload;
char rx = 'X'; //to read first byte from serial buffer

const char* ssid = "Dialog 4G 055";
const char* password = "6051A6D3";
const char* ip = "192.168.8.184";
const int port = 8011;

//WiFiServer server(80);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  //Display
  display.display();
  display.clearDisplay();
  Welcome();
  Index();
  //Serial pins
  //  pinMode(23, INPUT);
  //  pinMode(22, INPUT);
  //Serial com
  Serial.begin(115200);
  MySerial1.begin(115200, SERIAL_8N1, 2, 4); //https://quadmeup.com/arduino-esp32-and-3-hardware-serial-ports/
  Serial2.begin(115200);
  //Setting Serial Buffer size
  MySerial1.setRxBufferSize(1024);
  Serial2.setRxBufferSize(1024);

  //Display
  ConnectingWIFI();
  //Connecting to WiFi
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  //Starting server
  //    server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
  // server address, port and URL
  Serial.print("Connecting to IP:");
  Serial.println(ip);
  webSocket.begin(ip, port, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //  webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  ConnectingNode();
  webSocket.loop();
  Send_data();
  delay(1);
}

void Send_data()
{
  if (wsconnected)
  {
    Sending();
    Serial.println("connected to Nodejs");
    Serial.println("Sending data");

    while (1)
    {
      currentMillis = millis();
      while (currentMillis - previousMillis < interval)
      {
        currentMillis = millis();
        delayMicroseconds(5);
        readPressure();
        readBuffer(MySerial1);
        readPressure(); // include in readBuffer to increase Sampling
        readBuffer(Serial2);
      }
      previousMillis = currentMillis;
      removeCommas();


      payload = "{\"PData\":" + PData + "], \"SData\":" + SData + "] ,\"Stime\":" + Stime + "], \"BData\" : " + BData + "],\"TData\" : " + TData + "], \"EData\" : " + EData + "],\"PCData\":" + PCData +
      "], \"SCData\":" + SCData + "] , \"BCData\" : " + BCData + "], \"TCData\" : " + TCData + "], \"ECData\" : " + ECData + "]}";
//       payload = "{\"PData\":" + PData + "],\"Ptime\":" + Ptime + "], \"SData\":" + SData + "] ,\"Stime\":" + Stime + "], \"BData\" : " + BData + "],\"Btime\":" +
//                Btime + "], \"TData\" : " + TData + "], \"Ttime\":" + Ttime + "],\"EData\" : " + EData + "],\"Etime\":" + Etime +
//                "],\"PCData\":" + PCData + "],\"PCtime\":" + PCtime + "], \"SCData\":" + SCData + "] ,\"SCtime\":" + SCtime + "], \"BCData\" : " + BCData + "],\"BCtime\":" +
//                BCtime + "], \"TCData\" : " + TCData + "], \"TCtime\":" + TCtime + "],\"ECData\" : " + ECData + "],\"ECtime\":" + ECtime + "] }";
      Serial.println(payload);
      webSocket.sendTXT(payload);
      resetData();
      delay(10);
    }
  }
}
void readPressure()
{
  P_data.concat(String(analogRead(34)) + ",");
  delayMicroseconds(10);
}
// buffer reading function
boolean active = false;
String *combineTo;
String *combineTime;
String value = "";
void readBuffer(HardwareSerial buffReader)
{
    
  while (buffReader.available())
  { //Reading buffer
    rx = buffReader.read();
    Serial.print(rx);
    switch (rx)
    {
      case 'X':
        if (active)
        {
          active = false;
          *combineTo = *combineTo + value + ",";
          value = "";
//                    *combineTime = *combineTime + String(millis()) + ",";
        }
        break;
      case 'T':
        active = true;
        combineTo = &TData;
        combineTime = &Ttime;
        break;
      case 'E':
        active = true;
        combineTo = &EData;
        combineTime = &Ttime;
        break;
      case 'I':
        active = true;
        combineTo = &SData;
        combineTime = &Stime;
        break;
      case 'R':
        active = true;
        combineTo = &BData;
        combineTime = &Btime;
        break;
      case 'P':
        active = true;
        combineTo = &PData;
        combineTime = &Ptime;
        break;
//      case 't':
//        active = true;
//        combineTo = &TCData;
//        break;
//      case 'e':
//        active = true;
//        combineTo = &ECData;
//        break;
//      case 'i':
//        active = true;
//        combineTo = &SCData;
//        break;
//      case 'r':
//        active = true;
//        combineTo = &BCData;
//        break;
//      case 'p':
//        active = true;
//        combineTo = &PCData;
//        break;

      default:
        if (active && isDigit(rx))
        {
          value = value + rx;

        }
    }
  }
  //print state
  //  Serial.println();
  //  Serial.print("BData");
  //  Serial.println(BData);
  //  Serial.print("SData");
  //  Serial.println(SData);
  //  Serial.print("TData");
  //  Serial.println(TData);
  //  Serial.print("EData");
  //  Serial.println(EData);
}
void removeComma(String &memRef)
{
  if (memRef[memRef.length() - 1] == ',')
  {
    memRef.remove(memRef.length() - 1); //removing ending coma
  }
}
void removeCommas() {
  removeComma(P_data);
  removeComma(TData);
  removeComma(EData);
  removeComma(BData);
  removeComma(SData);
  removeComma(PData);
    removeComma(Ttime);
  removeComma(Etime);
  removeComma(Btime);
  removeComma(Stime);
  removeComma(Ptime);

  removeComma(TCData);
  removeComma(ECData);
  removeComma(BCData);
  removeComma(SCData);
  removeComma(PCData);
}
void resetData()
{
  Serial.println("Resetting");
  P_data = "[";
  TData = "[";
  EData = "[";
  SData = "[";
  BData = "[";
  PData = "[";
    Ttime = "[";
  Etime = "[";
  Btime = "[";
  Stime = "[";
  Ptime = "[";
 
  //  Control arm
  TCData = "[";
  ECData = "[";
  SCData = "[";
  BCData = "[";
  PCData = "[";

}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      wsconnected = false;
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      wsconnected = true;
      // send message to server when Connected
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      //      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }

}
//Functions for display
void configureDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0); // Start at to  p-left corner
  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
}
void Index(void)
{
  configureDisplay();
  display.println("160648A");
  display.println("160145E");
  display.println("160173L");
  display.println("160199V");
  display.display();
  delay(2000);
}

void Welcome(void)
{
  configureDisplay();
  display.println(" WELCOME!");
  display.println("  Final");
  display.println("   Year");
  display.println(" Project");
  display.display();
  delay(2000);
}
void ConnectingWIFI(void)
{
  configureDisplay();
  display.println("Connecting to WiFi:");
  display.println(String(ssid));
  display.display();
  //  delay(1000);
}
void ConnectingNode(void)
{
  configureDisplay();
  display.println("Connected to WiFi!");
  display.println("Connecting to NodeJS");
  display.println("Target IP:");
  display.println(String(ip));
  display.println("Target port:");
  display.println(String(port));
  display.display();
  //  delay(1000);
}

void Sending(void)
{
  configureDisplay();
  display.println("Connected!");
  display.println("Sending");
  display.println("data");
  display.display();
  //  delay(2000);
}
