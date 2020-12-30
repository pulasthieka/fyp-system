
uint8_t x=0; //temp
uint8_t y=128; // ecg
void setup() {
  Serial.begin(2000000);
}

void loop() {
  Serial.print("S"+String(int(y))+'X');//E
  Serial.print("B"+String(int(x))+'X');//T
  x=x+1;
  y=y-1;
  delay(10);
}
