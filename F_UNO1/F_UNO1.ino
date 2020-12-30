uint8_t x=0; //temp
uint8_t y=128; // ecg
void setup() {
  Serial.begin(57600);
}

void loop() {
  Serial.print("E"+String(y));
  Serial.print("T"+String(x));
  x=x-1;
  y=y+1;
  delay(10);
}
