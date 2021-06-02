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

long SavedTime; //For timing

void SerialPlot(char measurement, int value) {
  //  Serial.print(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
  Serial.print(measurement + String(value) + "X");
}
void SerialPlot(String measurement, long value) {
  //  Serial.print(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
  Serial.print(measurement + String(value) + "X");
}

void timeThis(String msg){
//  long timePeriod = micros()-SavedTime;
//  Serial.println(msg);
//  Serial.println(timePeriod);
//  SavedTime = micros();
}
