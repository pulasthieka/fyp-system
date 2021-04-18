#include <HardwareSerial.h>
HardwareSerial MySerial1(1);
int UART_SPEED= 115200;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(UART_SPEED);
  MySerial1.begin(UART_SPEED, SERIAL_8N1, 2, 4);

  Serial2.setRxBufferSize(1024);
  MySerial1.setRxBufferSize(1024);
  
  Serial.print("setup complete esp");
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial2.available()) {
    Serial.print(Serial2.read());
  }
  while (MySerial1.available()) {
    char rx = MySerial1.read();
    Serial.print(rx);
  }
}
