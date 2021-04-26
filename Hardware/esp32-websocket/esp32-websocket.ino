#include <Arduino.h>
#include <ArduinoJson.h> // for JSON documents

#include <SPI.h>
#include <Wire.h>
#include <HardwareSerial.h> // for setting up custom serial buffers

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>

#include <Adafruit_GFX.h> // Display libraries
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define DISPLAY_ADDR 0x3C

Adafruit_SSD1306 Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HardwareSerial MySerial1(1);
WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 200;
// int length_of_payload = 0;

//int capacity = 1024;
//const char json[] = "{\"PData\":[],\"SData\":[],\"BData\":[],\"TData\":[],\"EData\":[],\"PCData\":[],\"SCData\":[],\"BCData\":[],\"TCData\":[],\"ECData\":[],\"WData\":[],\"WCData\":[],\"CData\":[],\"CCData\":[],\"YData\":[],\"YCData\":[],\"ZData\":[],\"ZCData\":[],\"AData\":[],\"ACData\":[],\"FData\":[],\"FCData\":[],\"GData\":[],\"GCData\":[],\"HData\":[],\"HCData\":[],\"JData\":[],\"JCData\":[],\"KData\":[],\"KCData\":[]}";
//DynamicJsonDocument root(capacity);
String payload;
char rx = 'X'; //to read first byte from serial buffer

const char* ssid = "wifi";
const char* password = "pSW";
const char* ip = "192.168.1.12";
const int port = 8011;
bool wsconnected = false;

String TData = "[";
String EData = "[";
String SData = "[";
String BData = "[";
String PData = "[";
String TCData = "[";
//String ECData = "[";
String SCData = "[";
String BCData = "[";
String PCData = "[";
String WData = "[";
String WCData = "[";
String YData = "[";
String YCData = "[";
//String ZData = "[";
//String ZCData = "[";
//String AData = "[";
//String ACData = "[";
//String CData = "[";
//String CCData = "[";
//String FData = "[";
//String FCData = "[";
//String GData = "[";
//String GCData = "[";
//String HData = "[";
//String HCData = "[";
//String JData = "[";
//String JCData = "[";
//String KData = "[";
//String KCData = "[";


void setup()
{
  if (!Display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

  displayWelcomeMessage();

  Serial.begin(115200);
  MySerial1.begin(115200, SERIAL_8N1, 2, 4); //https://quadmeup.com/arduino-esp32-and-3-hardware-serial-ports/
  Serial2.begin(115200);

  MySerial1.setRxBufferSize(1024); //Setting Serial Buffer size for 1
  Serial2.setRxBufferSize(1024); //Setting Serial Buffer size for 2

  displayConnectingStatus();
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("ESP32 alive at ");
  Serial.println(WiFi.localIP());

  ConnectingNode();
  webSocket.begin(ip, port, "/");

  // Websockets
  webSocket.onEvent(webSocketEvent);
  //  webSocket.setAuthorization("user", "Password"); // use HTTP Basic Authorization this is optional remove if not needed
  webSocket.setReconnectInterval(5000); // try ever 5000 again if connection has failed

  //  deserializeJson(root, json); // configure JSON object
}

void loop()
{
  webSocket.loop();
  Send_data();
  delay(1);
}

void Send_data()
{
  if (wsconnected)
  {
    displayConnectedStatus(true);
    while (wsconnected)
    {
      currentMillis = millis();
      // length_of_payload = TData.length() + EData.length() + SData.length() + BData.length() + PData.length() + TCData.length() + SCData.length() + BCData.length() + PCData.length() + WData.length() + WCData.length() + YData.length() + YCData.length()  ;
      while (currentMillis - previousMillis < interval)
        //      while (length_of_payload < 1600)
      {
        currentMillis = millis();
        delayMicroseconds(5);
        readBuffer(MySerial1);
        readBuffer(Serial2);
        // length_of_payload = TData.length() + EData.length() + SData.length() + BData.length() + PData.length() + TCData.length() + SCData.length() + BCData.length() + PCData.length() + WData.length() + WCData.length() + YData.length() + YCData.length()  ;
      }
      previousMillis = currentMillis;
      //      while (currentMillis - previousMillis < 2*interval)
      //      {
      //        currentMillis = millis();
      //        delayMicroseconds(5);
      //        readBuffer(Serial2, root);
      //      }
      //      previousMillis = currentMillis;
      removeCommas();
      payload = "";
      //      payload = "{\"TData\":" + TData + "],\"EData\":" + EData + "],\"SData\":" + SData + "],\"BData\":" + BData + "],\"PData\":" + PData + "],\"TCData\":" + TCData + "],\"ECData\":" + ECData + "],\"SCData\":" + SCData + "],\"BCData\":" + BCData + "],\"PCData\":" + PCData + "],\"WData\":" + WData + "],\"WCData\":" + WCData + "],\"YData\":" + YData + "],\"YCData\":" + YCData + "],\"ZData\":" + ZData + "],\"ZCData\":" + ZCData + "],\"AData\":" + AData + "],\"ACData\":" + ACData + "],\"CData\":" + CData + "],\"CCData\":" + CCData + "],\"FData\":" + FData + "],\"FCData\":" + FCData + "],\"GData\":" + GData + "],\"GCData\":" + GCData + "],\"HData\":" + HData + "],\"HCData\":" + HCData + "],\"JData\":" + JData + "],\"JCData\":" + JCData + "],\"KData\":" + KData + "],\"KCData\":" + KCData + "] }" ;
      //        payload = "{\"TData\":" + TData + "],\"EData\":" + EData + "],\"SData\":" + SData + "],\"BData\":" + BData + "],\"PData\":" + PData + "],\"WData\":" + WData + "],\"WCData\":" + WCData + "] }" ;
      payload = "{\"TData\":" + TData + "],\"EData\":" + EData + "],\"SData\":" + SData + "],\"BData\":" + BData + "],\"PData\":" + PData + "],\"TCData\":" + TCData + "],\"SCData\":" + SCData + "],\"BCData\":" + BCData + "],\"PCData\":" + PCData + "],\"WData\":" + WData + "],\"WCData\":" + WCData + "],\"YData\":" + YData + "],\"YCData\":" + YCData + "]}" ;
      // serializeJson(root, Serial);//print to serial port
      Serial.println(payload.length());
      resetData();
      //      serializeJson(root, payload);
      webSocket.sendTXT(payload);
      //      deserializeJson(root, json);
      delay(10);
    }
  }
  else {
    displayConnectedStatus(false);
  }
}

// buffer reading function
boolean active = false;
String* combineTo;
String *combineTime;
String value = "";

void readBuffer(HardwareSerial buffReader)
{
  //  while (!active) {
  //Serial.print(".2!");
  active = false; //starting with a number
  while (buffReader.available() || active)
  { //Reading buffer
//    while (!buffReader.available()){delayMicroseconds(1);} 
    rx = char(buffReader.read());
    switch (rx)
    {
      case 'X':
        if (active)
        {
          *combineTo = *combineTo + value + ",";
          value = "";
          return;
        }
        active = false;
        break;
      case 'T':
        active = true;
        combineTo = &TData;
        value = "";
        break;
      case 'E':
        active = true;
        combineTo = &EData;
        value = "";
        break;
      case 'I':
        active = true;
        combineTo = &SData;
        value = "";
        break;
      case 'R':
        active = true;
        combineTo = &BData;
        value = "";
        break;
      case 'P':
        active = true;
        combineTo = &PData;
        value = "";
        break;
      case 't':
        active = true;
        combineTo = &TCData;
        value = "";
        break;
      //      case 'e':
      //        active = true;
      //        combineTo = &ECData;
      //        break;
      case 'i':
        active = true;
        combineTo = &SCData;
        value = "";
        break;
      case 'r':
        active = true;
        combineTo = &BCData;
        value = "";
        break;
      case 'p':
        active = true;
        combineTo = &PCData;
        break;
      case 'A':
        active = true;
        combineTo = &WData;
        value = "";
        break;
      case 'a':
        active = true;
        combineTo = &WCData;
        value = "";
        break;
      case 'B':
        active = true;
        combineTo = &YData;
        value = "";
        break;
      case 'b':
        active = true;
        combineTo = &YCData;
        value = "";
        break;
      //      case 'O':
      //        active = true;
      //        combineTo = &ZData;
      //        break;
      //      case 'P':
      //        active = true;
      //        combineTo = &ZCData;
      //        break;
      //      case 'Q':
      //        active = true;
      //        combineTo = &AData;
      //        break;
      //      case 'R':
      //        active = true;
      //        combineTo = &ACData;
      //        break;
      //      case 'S':
      //        active = true;
      //        combineTo = &CData;
      //        break;
      //      case 'T':
      //        active = true;
      //        combineTo = &CCData;
      //        break;
      //      case 'U':
      //        active = true;
      //        combineTo = &FData;
      //        break;
      //      case 'V':
      //        active = true;
      //        combineTo = &FCData;
      //        break;
      //      case 'W':
      //        active = true;
      //        combineTo = &GData;
      //        break;
      //      case 'Y':
      //        active = true;
      //        combineTo = &GCData;
      //        break;
      //      case 'Z':
      //        active = true;
      //        combineTo = &HData;
      //        break;
      //      case '*':
      //        active = true;
      //        combineTo = &HCData;
      //        break;
      //      case '(':
      //        active = true;
      //        combineTo = &JData;
      //        break;
      //      case ')':
      //        active = true;
      //        combineTo = &JCData;
      //        break;
      //      case '+':
      //        active = true;
      //        combineTo = &KData;
      //        break;
      //      case '-':
      //        active = true;
      //        combineTo = &KCData;
      //        break;
      default:
        if (active && isDigit(rx))
        {
          value = value + rx;
        }
    }
  }

  //  }
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
    case WStype_ERROR:
      Serial.printf("[WSc] Error \n");
      break;
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }

}
void removeCommas() {
  removeComma(TData);
  removeComma(EData);
  removeComma(SData);
  removeComma(BData);
  removeComma(PData);
  removeComma(TCData);
  //  removeComma(ECData);
  removeComma(SCData);
  removeComma(BCData);
  removeComma(PCData);
  removeComma(WData);
  removeComma(WCData);
  removeComma(YData);
  removeComma(YCData);
  //  removeComma(ZData);
  //  removeComma(ZCData);
  //  removeComma(AData);
  //  removeComma(ACData);
  //  removeComma(CData);
  //  removeComma(CCData);
  //  removeComma(FData);
  //  removeComma(FCData);
  //  removeComma(GData);
  //  removeComma(GCData);
  //  removeComma(HData);
  //  removeComma(HCData);
  //  removeComma(JData);
  //  removeComma(JCData);
  //  removeComma(KData);
  //  removeComma(KCData);
}

void removeComma(String &memRef)
{
  if (memRef[memRef.length() - 1] == ',')
  {
    memRef.remove(memRef.length() - 1); //removing ending coma
  }
}
void resetData()
{
  //    Serial.println("Resetting");

  TData = "[";
  EData = "[";
  SData = "[";
  BData = "[";
  PData = "[";
  TCData = "[";
  //  ECData = "[";
  SCData = "[";
  BCData = "[";
  PCData = "[";
  WData = "[";
  WCData = "[";
  YData = "[";
  YCData = "[";
  //  ZData = "[";
  //  ZCData = "[";
  //  AData = "[";
  //  ACData = "[";
  //  CData = "[";
  //  CCData = "[";
  //  FData = "[";
  //  FCData = "[";
  //  GData = "[";
  //  GCData = "[";
  //  HData = "[";
  //  HCData = "[";
  //  JData = "[";
  //  JCData = "[";
  //  KData = "[";
  //  KCData = "[";
}

//Functions for display
void configureDisplay()
{
  Display.clearDisplay();
  Display.setCursor(0, 0); // Start at to  p-left corner
  Display.setTextSize(1);  // Draw 2X-scale text
  Display.setTextColor(SSD1306_WHITE);
}

void displayWelcomeMessage(void)
{
  configureDisplay();
  Display.println("WELCOME!");
  Display.println("Final Year");
  Display.println("Project");
  Display.println("160648A");
  Display.println("160145E");
  Display.println("160173L");
  Display.println("160199V");
  Display.display();
  delay(2000);
}

void displayConnectingStatus(void)
{
  Serial.println();
  Serial.println("Connecting to: " + String(ssid));
  configureDisplay();
  Display.println("Connecting to WiFi:");
  Display.println(String(ssid));
  Display.display();
  //  delay(1000);
}

void ConnectingNode(void)
{
  Serial.println("Connecting to IP: " + String(ip));
  configureDisplay();
  Display.println("Connected to WiFi!");
  Display.println("Connecting to NodeJS");
  Display.println("Target IP:");
  Display.println(String(ip));
  Display.println("Target port:");
  Display.println(String(port));
  Display.display();
  //  delay(1000);
}

void displayConnectedStatus(boolean connStatus)
{
  configureDisplay();
  Display.println("Connected? " + connStatus);
  if (connStatus) {
    Serial.println("connected to Nodejs");
    Serial.println("Sending data");
    Display.println("Sending");
    Display.println("data");
  }
  else {
    Serial.println("Nodejs Disconnected");
    Display.println("Disconnected");
  }
  Display.display();
  //  delay(2000);
}
