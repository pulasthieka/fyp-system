//Functions for PAT
bool setupPat() {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write((uint8_t) ADS1115_REG_POINTER_CONFIG);
  // Wire.write((uint8_t*) value, 2);
  uint16_t FSR = FSR_2V;
  Wire.write((uint8_t)(FSR >> 8));
  Wire.write((uint8_t)(FSR & 0xFF));
  return (Wire.endTransmission()==0);
}

uint16_t readDataPat() {
  transmitData(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT, -1, true); 
  uint8_t pat = Wire.requestFrom(ADS1115_ADDRESS, (uint8_t)2);
  return ((pat[0] << 8) | pat[1]);
}
