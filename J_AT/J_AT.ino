#include "SoftwareSerial.h"

SoftwareSerial Monitor(5, 1);

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
