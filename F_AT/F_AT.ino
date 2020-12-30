#include "SoftwareSerial.h"

SoftwareSerial Monitor(5, 4);

uint8_t x=0; //temp
uint8_t y=128; // ecg
void setup() {
  Monitor.begin(38400);
}

void loop() {
  Monitor.print("S"+String(int(y)));//E
  Monitor.print("B"+String(int(x)));//T
  x=x+2;
  y=y-2;
  delay(10);
}
