#include <Wire.h>
#include <STSSensor.h>

#define AMBIENT A3 //toggle address
#define BODY A2

STSSensor body(Wire, 1);
STSSensor ambient(Wire, 2);

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
    ambient.init(Wire);
}

void loop()
{
    body.readSample();
    Serial.print("T" + String(body.getTemperature()) + 'X'); //E
    ambient.readSample();
    Serial.print("E" + String(ambient.getTemperature()) + 'X'); //T
}