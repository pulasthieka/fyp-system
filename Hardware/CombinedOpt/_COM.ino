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

int SerialPlot(String measurement, int value) {
  //  Serial.print(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
  Serial.print(measurement + String(value) + "X");
  return 0;
}
int SerialPlot(char measurement, long value) {
  //  Serial.print(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
  Serial.print(measurement + String(value) + "X");
  return 0;
}

void timeThis(String msg){
//  long timePeriod = micros()-SavedTime;
//  Serial.print(msg);
//  Serial.println(timePeriod);
//  SavedTime = micros();
}
