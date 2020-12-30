//variables
int PAT=0; //for PAT signal
String server=""; //what we send to server
String serial=""; //to store serial buffer
char rx; //to read first byte from serial buffer
void setup() {  
  Serial.begin(57600);
}

void loop() {
  //Empty seral buffer
  serial="";
  while(1){ //componsate for data fully not recieved
  while(Serial.available()){              
  rx=Serial.read();
  serial.concat(rx);        }
  delayMicroseconds(250);
  if(!Serial.available()){break;}
  }
  server.concat(serial);
  //Read PAT signal
  PAT = analogRead(A0);
  server.concat("P"+String(PAT));

  
  Serial.println(server); //emulates sending HTML post
  server="";
  delay(5);
}
