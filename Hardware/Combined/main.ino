float temperatureC ;
bool isAvailablePAT = false;
bool isAvailableTemp = false;

int16_t PAT = 0;
long SavedTime; //For timing
String message = "{";

void setup() {
  Wire.begin();
  Serial.begin(115200);
  PPGbegin();
  delay(10); //Max powerup time
}

void loop() {
  /* refer to page 28 of datasheet
    https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  isAvailablePAT = writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)

  //GET Temperature
  //  SavedTime = micros();
  //  requestTemp(BodyTempAdd);
  if ( requestTemp(EnvTempAdd)) {
    temperatureC = getTemp(EnvTempAdd);
    SerialPlot("t", 100*temperatureC);
    message = message + "\"TData\":[" + temperatureC + "],\"Ttime\":[" + millis() + "],";
  }

  //  GET Pressure
  //  SavedTime = micros();
  if (isAvailablePAT) {
    PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT);
    SerialPlot("p", PAT);
    writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
    message = message + "\"hello\":[" + PAT + "],\"Ptime\":[" + millis() + "],";
  }

  // GET ppg values
  SavedTime = micros();
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  SerialPlot("i", RED );
  SerialPlot("r", IR );
  message = message + "\"SData\":[" + RED + "],\"Stime\":[" + millis() + "],";
  message = message + "\"BData\":[" + IR + "],\"Btime\":[" + millis() + "],";
  ////getimp(0x18, 0x6A, 0x73,'R');
  message = message + "}";
  //  webSocket.sendTXT(message);
  message = "{";
}
