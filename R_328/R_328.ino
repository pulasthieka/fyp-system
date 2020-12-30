#include <Wire.h>
#include <STSSensor.h>
#define AMBIENT A3
#define BODY  A2
uint8_t address2= 0x4B;
STSSensor body(Wire);
STSSensor ambient(Wire, 2);
void setup() {
//    Serial.println("T199999X");
  pinMode(AMBIENT,OUTPUT);
  pinMode(BODY,OUTPUT);
  digitalWrite(AMBIENT,0);
  digitalWrite(BODY,1);
  Wire.begin();
//  Wire.setClock(400000);
  Serial.begin(2000000);
  body.init(Wire);
  ambient.init(Wire);

}

void loop() {
  body.readSample();
  Serial.print("T"+String(body.getTemperature())+'X');//E,S
  ambient.readSample();
  Serial.print("E"+String(ambient.getTemperature())+'X');//T,B
//  delay(100);
//  Serial.println("E9X");
}
