int x;
void setup() {
  //pin 13 is used to time the nodeMCU
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(5000);
  Serial.begin(57600);
  delay(10);
  for (x=0;x<100;x++){
    Serial.print(String(x)+"e");
    }
  delay(10);
  digitalWrite(13,LOW); //Signaling nodeMCU that communication is complete
   
}

void loop() {

}
