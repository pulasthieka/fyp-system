//Connect pin 3 (3 from upper left) to pin RX2 of ESP32
//            _________
//    Pin 1 --| o     |-- Pin 8
//    Pin 2 --|       |-- Pin 7
//    Pin 3 --|       |-- Pin 6
//    Pin 4 --|_______|-- Pin 5
//

#include "SoftwareSerial.h"

SoftwareSerial Monitor(5, 4);

uint8_t x=0; //temp
uint8_t y=128; // ecg
void setup() {
  pinMode(1,OUTPUT); //Clear to read for esp32
digitalWrite(1,LOW);
  Monitor.begin(38400);
}

void loop() {
  digitalWrite(1,HIGH);
  Monitor.print("S"+String(int(y))+'X');//E
  Monitor.print("B"+String(int(x))+'X');//T
  digitalWrite(1,LOW);
  x=x+1;
  y=y-1;
  delay(10);
}
