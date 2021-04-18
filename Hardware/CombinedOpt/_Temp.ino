byte byte1, byte2, byte3;
float tC;
//Functions for temperature sensors
bool requestTemp(int address) {
  // set up for single shot mode / clock stretching /.5/sec max frequency
  Wire.beginTransmission(address);
  //start the communication with IC  with the address
  Wire.write(byte(0x2C));
  Wire.write(byte(0x06));
  return (Wire.endTransmission() == 0);
}
float getTemp(int address) {
  // request the 3 bytes of measurement data 16 bit temp value plus checksum
  Wire.requestFrom(address, 3);
  byte1 = Wire.read(); // msb of temp
  byte2 = Wire.read(); // lsb of temp
  byte3 = Wire.read(); // CRC check
  //  Serial.print("CRC = ");
  //  Serial.println(byte3);
  tC = (byte1 << 8) + byte2;
  return ( -45  + (175 * (float)tC / 65535)); //temperature conversion
}
