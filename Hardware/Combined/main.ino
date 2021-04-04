float temperatureC ;
bool isAvailablePAT = false;
bool isAvailableTemp = false;

int16_t PAT = 0;
long SavedTime; //For timing
String message = "{";
#define PWM_PIN A3

void setup() {
  Wire.begin();
  pinMode(PWM_PIN, OUTPUT);
  Serial.begin(2000000);
  PPGbegin();
  delay(10); //Max powerup time
  while (!Serial);
  Wire.beginTransmission(device_address);
  if (!Wire.endTransmission()) {
    Serial.println("AD5933 found");
  }
  else {
    Serial.println("Error - AD5933 not found");
    //    while (1) {
    //      delay(1);
    //      //will not pass this point if AD5933 is not connected
    //    }
  }
}

void loop() {
  /* refer to page 28 of datasheet
    https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  isAvailablePAT = writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles);
  waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
  //  printImp('W','w'); //Getting the sample from AD5933 and serial printing
  //  waitTillImpReady();
  //  printImp('Y','y');
  //  waitTillImpReady();
  //  printImp('Z','z');
  //  waitTillImpReady();
  //  printImp('A','a');
  //  waitTillImpReady();
  //  printImp('C','c');

  printImp('F', 'f'); //Getting the sample from AD5933 and serial printing
  waitTillImpReady();
  printImp('G', 'g');
  waitTillImpReady();
  printImp('H', 'h');
  waitTillImpReady();
  printImp('J', 'j');
  waitTillImpReady();
  printImp('K', 'k');

  //GET Temperature
  //  SavedTime = micros();
  //  requestTemp(BodyTempAdd);
  if ( requestTemp(BodyTempAdd)) {
    temperatureC = getTemp(BodyTempAdd);
    SerialPlot("T", 100 * temperatureC);
    //    message = message + "\"TData\":[" + temperatureC + "],\"Ttime\":[" + millis() + "],";
  }
  if ( requestTemp(EnvTempAdd)) {
    temperatureC = getTemp(EnvTempAdd);
    SerialPlot("E", 100 * temperatureC);
    //    message = message + "\"TData\":[" + temperatureC + "],\"Ttime\":[" + millis() + "],";
  }

  //  GET Pressure
  //  SavedTime = micros();
  if (isAvailablePAT) {
    PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT);
    SerialPlot("P", PAT);
    if (PAT < 20000) {
      digitalWrite(PWM_PIN, HIGH);
    }    if (PAT > 30000) {
      digitalWrite(PWM_PIN, LOW);
    }
    writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
    message = message + "\"hello\":[" + PAT + "],\"Ptime\":[" + millis() + "],";
  }

  // GET ppg values
  SavedTime = micros();
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  SerialPlot("I", RED );
  SerialPlot("R", IR );
  message = message + "\"SData\":[" + RED + "],\"Stime\":[" + millis() + "],";
  message = message + "\"BData\":[" + IR + "],\"Btime\":[" + millis() + "],";
  ////getimp(0x18, 0x6A, 0x73,'R');
  message = message + "}";
  //  webSocket.sendTXT(message);
  message = "{";
}
