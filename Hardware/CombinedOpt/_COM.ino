//#include <Arduino.h>
//
//#include <WiFi.h>
//#include <WiFiMulti.h>
//#include <WiFiClientSecure.h>
//#include <WebSocketsClient.h>

//Status LEDs
const int STATUS_LED1 = 5;
const int STATUS_LED2 = 6;

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

void timeThis(String msg) {
  //  long timePeriod = micros()-SavedTime;
  //  Serial.println(msg);
  //  Serial.println(timePeriod);
  //  SavedTime = micros();
}

void showStatus(int state) {
  switch (state) {
    case 1: // Ready
      digitalWrite(STATUS_LED1, LOW);
      digitalWrite(STATUS_LED2, LOW);
      break;
    case 2: // Request start
      digitalWrite(STATUS_LED1, LOW);
      digitalWrite(STATUS_LED2, HIGH);
      break;
    case 3: //Request end
      digitalWrite(STATUS_LED1, HIGH);
      digitalWrite(STATUS_LED2, LOW);
      break;
    default: // Powered On stuck
      digitalWrite(STATUS_LED1, HIGH);
      digitalWrite(STATUS_LED2, HIGH);
  }
}
