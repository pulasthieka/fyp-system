#include <HardwareSerial.h>
HardwareSerial MySerial1(1);
char x;
char y;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MySerial1.begin(115200, SERIAL_8N1, 2, 4);
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()) {
    Serial.print("d");
    y = Serial2.read();
    Serial.print(y);
  }
  if (MySerial1
      .available()) {
    x = MySerial1.read();
    Serial.print(x);
  }
}
