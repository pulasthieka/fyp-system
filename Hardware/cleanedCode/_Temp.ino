byte byte1, byte2, byte3;
float tC;

byte TEMP_subAddress = byte(0x2C);
byte TEMP_value = byte(0x06);

//Functions for temperature sensors

float getTemp(int address) {
  // request the 3 bytes of measurement data 16 bit temp value plus checksum
  Wire.requestFrom(address, 3);
  byte1 = Wire.read(); // msb of temp
  byte2 = Wire.read(); // lsb of temp
  byte3 = Wire.read(); // CRC check
  tC = (byte1 << 8) + byte2;
  return ( -45  + (175 * (float)tC / 65535)); //temperature conversion
}