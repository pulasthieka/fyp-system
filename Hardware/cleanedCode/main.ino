float temperatureC ;
bool isAvailablePAT = false;
bool isAvailableTemp = false;

int16_t PAT = 0;
long SavedTime; //For timing
String message = "{";

void setup() {
  
  Wire.begin();
  Serial.begin(115200);
  Serial.println("setup starting");
  PPGbegin();
  delay(10); //Max powerup time
  Serial.println("setup done");
}

void loop() {
  /* refer to page 28 of datasheet
    https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  Serial.println("check pat");
  isAvailablePAT = setupPat();

  //GET Temperature
  Serial.println("get temp");
  if ( transmitData(EnvTempAdd, TEMP_subAddress, TEMP_value, true)) {
    temperatureC = getTemp(EnvTempAdd);
    SerialPlot("T", 100*temperatureC);
  }
Serial.println("get pat");
  //  GET Pressure
  if (isAvailablePAT) {
    PAT = readDataPat();
    SerialPlot("P", PAT);
    setupPat(); 
  }
Serial.println("get PPG");
  // GET ppg values 
  if (!checkStatusPpg()){
    PPGbegin();
  }
  Serial.println("PPG Checked");
  SavedTime = micros();
  uint8_t* temp;  // Temporary buffer for read values
  temp = readDataPpg(4);  // Read four times from the FIFO
  Serial.println("read PPG");
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  SerialPlot("I", RED );
  SerialPlot("R", IR );
} 
