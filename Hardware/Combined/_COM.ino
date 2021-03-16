//#include <Arduino.h>
//
//#include <WiFi.h>
//#include <WiFiMulti.h>
//#include <WiFiClientSecure.h>
//#include <WebSocketsClient.h>


// DATA Vars
String TData = "[";
String EData = "[";
String BData = "[";
String SData = "[";
String PData = "[";

//time Vars
String Ttime = "[";
String Etime = "[";
String Btime = "[";
String Stime = "[";
String Ptime = "[";

String payload;

int SerialPlot(String measurement, int value) {
  //  Serial.print(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
  Serial.print(measurement + String(value) + "X");
  return 0;
}
