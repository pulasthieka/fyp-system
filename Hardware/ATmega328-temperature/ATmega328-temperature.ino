#include <Wire.h>
#include <STSSensor.h>
#include <Adafruit_ADS1015.h>

//Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

#define AMBIENT A3 //toggle address
#define BODY A2

int16_t adc0;
STSSensor body(Wire, 1);
//STSSensor ambient(Wire, 2);

void setup()
{
  pinMode(AMBIENT, OUTPUT);
  pinMode(BODY, OUTPUT);
  digitalWrite(AMBIENT, 0);
  digitalWrite(BODY, 1);
  Wire.begin();
  //  Wire.setClock(400000); // for Fast I2C
  Serial.begin(2000000);
  body.init(Wire);
  //  ambient.init(Wire);
  //  ads.setGain(GAIN_TWO);
  //  ads.begin();
}

void loop()
{
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  body.readSample();
  Serial.print("T" + String(body.getTemperature()) + 'X'); //E
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //    adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  adc0 = ads.readADC_SingleEnded(0);
  //  Serial.println("P" + String(adc0) + 'X');
  //  ambient.readSample();
  //  Serial.print("E" + String(ambient.getTemperature()) + 'X'); //T
}
