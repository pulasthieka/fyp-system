#include "SoftwareSerial.h"

SoftwareSerial Monitor(5, 4);
uint8_t x =0;  //use byte and 8 bit ascii for letters, or use String()
void setup() {
  
Monitor.begin(57600); 
pinMode(0, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, INPUT);
}

void loop() {
  x=x+1;
  Monitor.print(String(int(x))+"e");//"LED : "+
  delay(1);
}
