char rx='a';
String val; //max length is 4096 bytes
void setup()  
{
  pinMode(D0,INPUT_PULLUP); // connect to 13 of UNO
  Serial.begin(57600);
  Serial.println("");
  Serial.println("Node Ready");
  while(digitalRead(D0)){ Serial.println("Waiting");delay(3000);}
  Serial.println("Triggered");  
}
 
void loop() // run over and over
{
    
    val="";
    
    while(Serial.available()){              
    rx=Serial.read();
    val.concat(rx);  
    }
    Serial.print(val);
    while(1){delay(1000);}        
       

}
