
#include <Wire.h>
#define ADS1115_ADDRESS (0x48)
#define ADS1115_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1115_CONVERSIONDELAY (9) ///< Conversion delay
#define ADS1015_REG_POINTER_CONVERT (0x00)   ///< Conversion

#define PWM_PIN A3
//#define VALVE 5
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

int16_t PAT;
void setup(void)
{
  // put your setup code here, to run once:
  pinMode(PWM_PIN, OUTPUT);
//  pinMode (VALVE, OUTPUT);
  //  ledcSetup(ledChannel, freq, resolution);
  //  ledcAttachPin(PWM_PIN, ledChannel);
  Serial.begin(115200);
  Wire.begin();
  delay(10);

}

void loop(void)
{

  /* refer to page 28 of datasheet
      https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  delay(ADS1115_CONVERSIONDELAY);
  PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT);
//  analogWrite(PWM_PIN, 0);
//  delay(2000);
//  analogWrite(PWM_PIN, 256);
//  delay(2000);
  if (PAT > 1000) {
    Serial.println(PAT);
    if (PAT > 30000) {
      //        ledcWrite(ledChannel, 0);
      analogWrite(PWM_PIN, 0);
//      delay(5000);
//      digitalWrite(VALVE, LOW);
//      delay(1000);
    } if (PAT < 20000) {
//      digitalWrite(VALVE, HIGH);
      //        ledcWrite(ledChannel, 256);
      analogWrite(PWM_PIN, 255);
      delay(10);
    }

  }}

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
