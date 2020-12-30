//#include<SoftwareSerial.h> //Included SoftwareSerial Library

//SoftwareSerial mySerial(13, 15);
 
char rx;
void setup()  
{
  Serial.begin(38400);

//  mySerial.begin(115200, SWSERIAL_8N1);  
}
 
void loop() // run over and over
{
//  if (mySerial.available()){
//        rx=mySerial.read();
//        Serial.println(rx);}
  if (Serial.available()){
    rx=Serial.read();
    Serial.print(rx);
    }
}
