float temperatureC ;
bool isAvailablePAT = false;
bool isAvailableTemp = false;

int16_t PAT = 0;
String message = "{";
#define PWM_PIN A3

void setup() {
  Wire.begin();
  pinMode(PWM_PIN, OUTPUT);
  Serial.begin(115200);
  PPGbegin();
  delay(10); //Max powerup time
  while (!Serial);
  Serial.println("setup complete ATMEGA");
//  checkAD5933();
}

void loop() {
  /* refer to page 28 of datasheet
    https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1611997309467&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS1115%253FkeyMatch%253DADS1115%2526tisearch%253DSearch-EN-everything%2526usecase%253DGPN
  */
  timeThis("1:Loop start:");

  if (checkAD5933()) {
    requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles);
    timeThis("3:Imp Requested:");

    boolean isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
    boolean isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    if ( isBodyTempAvailable) {
      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
      SerialPlot("T", 100 * temperatureC);
    }
    timeThis("16:communicate body temp:");

    if ( isEnvTempAvailable) {
      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
      SerialPlot("E", 100 * temperatureC);
    }
    timeThis("19:communicate env temp:");

    isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
    isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    if ( isBodyTempAvailable) {
      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
      SerialPlot("T", 100 * temperatureC);
    }
    timeThis("16:communicate body temp:");

    if ( isEnvTempAvailable) {
      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
      SerialPlot("E", 100 * temperatureC);
    }
    timeThis("19:communicate env temp:");

    isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
    isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    getPAT();
    getPPG();
    if ( isBodyTempAvailable) {
      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
      SerialPlot("T", 100 * temperatureC);
    }
    timeThis("16:communicate body temp:");

    if ( isEnvTempAvailable) {
      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
      SerialPlot("E", 100 * temperatureC);
    }
    timeThis("19:communicate env temp:");

    //    waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
    //  printImp('W','w'); //Getting the sample from AD5933 and serial printing
    //  waitTillImpReady();
    //  printImp('Y','y');
    //  waitTillImpReady();
    //  printImp('Z','z');
    //  waitTillImpReady();
    //  printImp('A','a');
    //  waitTillImpReady();
    //  printImp('C','c');
    waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
    timeThis("4:idle imp 1:");
    printImp('F', 'f'); //Getting the sample from AD5933 and serial printing
    timeThis("5:reading imp 1:");
//
//       isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
//    isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    if ( isBodyTempAvailable) {
//      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
//      SerialPlot("T", 100 * temperatureC);
//    }
//    timeThis("16:communicate body temp:");
//
//    if ( isEnvTempAvailable) {
//      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
//      SerialPlot("E", 100 * temperatureC);
//    }
//    timeThis("19:communicate env temp:");
//     
//    waitTillImpReady(); timeThis("6:idle imp 2:");
//    printImp('G', 'g'); timeThis("7:reading imp 2:");
//
//        isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
//    isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//
//    waitTillImpReady(); timeThis("8:idle imp 3:");
//    printImp('H', 'h'); timeThis("9:reading imp 3:");
//    
//    if ( isBodyTempAvailable) {
//      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
//      SerialPlot("T", 100 * temperatureC);
//    }
//    timeThis("16:communicate body temp:");
//
//    if ( isEnvTempAvailable) {
//      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
//      SerialPlot("E", 100 * temperatureC);
//    }
//    timeThis("19:communicate env temp:");
//
//           isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis("14:request body temp:");
//    isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis("17:request env temp:");
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//    
//    waitTillImpReady(); timeThis("10:idle imp 4:");
//    printImp('J', 'j'); timeThis("11:reading imp 4:");
//    
//    getPAT();
//    getPPG();
//    getPAT();
//    getPPG();
//
//    if ( isBodyTempAvailable) {
//      temperatureC = getTemp(BodyTempAdd); timeThis("15:received body temp:");
//      SerialPlot("T", 100 * temperatureC);
//    }
//    timeThis("16:communicate body temp:");
//
//    if ( isEnvTempAvailable) {
//      temperatureC = getTemp(EnvTempAdd); timeThis("18:received env temp:");
//      SerialPlot("E", 100 * temperatureC);
//    }
//    timeThis("19:communicate env temp:");
//
//    waitTillImpReady(); timeThis("12:idle imp 5:");
//    printImp('K', 'k'); timeThis("13:reading imp 5:");
  }

}
int getPPG() {
  // GET ppg values
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  timeThis("24:read PPG:");
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  SerialPlot("I", RED );
  SerialPlot("R", IR );
  timeThis("25:communicate PPG:");
  return 0;
}
int getPAT() {
  //  GET Pressure
  isAvailablePAT = writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  timeThis("2:PAT ADC set:");
  if (isAvailablePAT) {
    timeThis("20:check PAT status:");
    PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT); timeThis("21:read PAT reg:");
    SerialPlot("P", PAT); timeThis("22:communicate PAT :");
    if (PAT < 20000) {
      digitalWrite(PWM_PIN, HIGH);
    }    if (PAT > 30000) {
      digitalWrite(PWM_PIN, LOW);
    }
    timeThis("23:fire PAT motor:");
    //    writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
    //    timeThis("2:PAT ADC set:");
  }
}
