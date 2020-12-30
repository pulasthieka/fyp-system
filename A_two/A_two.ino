#include <Wire.h>

void setup() {
  Wire.begin(0x0A);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent);    // register event
  Serial.begin(115200);            // start serial to observe output
  pinMode(LED_BUILTIN, OUTPUT);    // interrupt pin for ATTiny
}

void loop() {                      // interrupting ATTiny
  digitalWrite(LED_BUILTIN, LOW);   
  delayMicroseconds(1);             
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1);                       
}

void receiveEvent(int howMany) { // printing the reading
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read();        // receive byte as a character
    Serial.print(c);             // print the character
  }
  int x = Wire.read();           // receive byte as an integer
  Serial.println(x);             // print the integer
}
