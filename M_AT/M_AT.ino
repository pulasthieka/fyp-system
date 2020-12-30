#include "SoftwareSerial.h"
#include <TinyWireM.h>
SoftwareSerial Monitor(5, 4);
int device_address = 8;
char x='i';




  
void setup() {
  TinyWireM.begin();
  Monitor.begin(38400);
  delay(3000);
}

void loop() {
  TinyWireM.requestFrom(device_address, 1);
  x=TinyWireM.receive();
  Monitor.print(x);
  delay(100);
}
