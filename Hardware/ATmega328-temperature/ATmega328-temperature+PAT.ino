#include <Wire.h>

/*****************Ground the ADDR pin of the body temperature sensor**************************/
#define BodyTempAdd 0x4A      //Prefix = T
#define EnvTempAdd 0x4B       //Prefix = E 
byte byte1, byte2, byte3;
float tC;
float temperatureC;

#define ADS1115_ADDRESS (0x48)                //Prefix = P
#define ADS1115_REG_POINTER_CONFIG (0x01)     // Configuration
#define ADS1115_CONVERSIONDELAY (9)           // Conversion delay
#define ADS1015_REG_POINTER_CONVERT (0x00)    // Conversion
int16_t PAT = 0;              

long SavedTime; //For timing

void setup() {
  Wire.begin();
  Serial.begin(2000000);
  delay(10); //Max powerup time
}

void loop() {
  /* refer to page 28 of datasheet
    https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  SavedTime = micros();
  requestTemp(BodyTempAdd);
  requestTemp(EnvTempAdd);
  while (SavedTime - micros() < 8923) {
    delayMicroseconds(1); //ADS1115_CONVERSIONDELAY
  }
  PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT);
  Serial.print("P" + String(PAT) + 'X');
  writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  
  
  temperatureC=getTemp(BodyTempAdd);
  Serial.print("T" + String(temperatureC) + 'X');
  temperatureC=getTemp(EnvTempAdd);
  Serial.print("E" + String(temperatureC) + 'X');
  SavedTime = micros();
  while (SavedTime - micros() < 4746) {
    delayMicroseconds(1); //ADS1115_CONVERSION_DELAY - STS_Read_Time
  }
  PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT);
  Serial.print("P" + String(PAT) + 'X');
}

//Functions for temperature sensors
void requestTemp(int address) {
  // set up for single shot mode / clock stretching /.5/sec max frequency
  Wire.beginTransmission(address);
  //start the communication with IC  with the address
  Wire.write(byte(0x2C));
  Wire.write(byte(0x06));
  Wire.endTransmission();
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


//Functions for PAT
void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  Wire.beginTransmission(i2cAddress);
  i2cwrite((uint8_t)reg);
  i2cwrite((uint8_t)(value >> 8));
  i2cwrite((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}
uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  Wire.beginTransmission(i2cAddress);
  i2cwrite(reg);
  Wire.endTransmission();
  Wire.requestFrom(i2cAddress, (uint8_t)2);
  return ((i2cread() << 8) | i2cread());
}
uint8_t i2cread(void) {
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}
void i2cwrite(uint8_t x) {
#if ARDUINO >= 100
  Wire.write((uint8_t)x);
#else
  Wire.send(x);
#endif
}
