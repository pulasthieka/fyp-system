//#include<SoftwareSerial.h> //Included SoftwareSerial Library

//SoftwareSerial mySerial(13, 15);
uint8_t y=100;
char rx='a';
String val;
void setup()  
{
  Serial.begin(57600);

//  mySerial.begin(115200, SWSERIAL_8N1);  
}
 
void loop() // run over and over
{
    while (!Serial.available()){}
    val="";
    while(1){
              while(Serial.available())       {              
              rx=Serial.read();
              if(rx!='e'){val.concat(rx);}  }
              if (rx=='e'){Serial.print(val);break;}      // "e" is the terminating character        
              }    
    Serial.println("  "+String(int(y)));
    y=y-1;
}
