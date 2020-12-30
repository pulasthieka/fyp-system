#include <HardwareSerial.h>
char rx;
void setup()  
{
  Serial.begin(115200);
//  Serial1.begin(2000000);
  Serial2.begin(2000000);
Serial.println("Serial test");
}
 
void loop() 
{
//  if (Serial1.available()){
//    rx=Serial1.read();
//    Serial.print(rx);
//    }


}
