float temperatureC ;
bool isAvailablePAT = false;
bool isAvailableTemp = false;

int16_t PAT = 0;
String message = "{";
#define PWM_PIN A3
boolean toggle1 = 0;

const char PAT_CHAR = 'p';
const char RED_CHAR = 'r';
const char IR_CHAR = 'i';
const char BODY_TEMP_CHAR = 't';
const char ENV_TEMP_CHAR = 'E';
const String IMP_REAL = "A";
const String IMP_IMG = "a";

void setup() {
  Wire.begin();
  pinMode(PWM_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(2000000);
  PPGbegin();
  delay(10); //Max powerup time
  while (!Serial);

  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = 19;// = (16*10^6) / (SAMPLING_RATE*1024) - 1 (must be <65536) 
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

void loop() {}

boolean isBodyTempAvailable;
boolean isEnvTempAvailable;
int timer_count = 0 ;
const int IMP_RATIO = 5;
const int TEMP_RATIO = 10;
int event_scaler0 = 0;
int event_scaler1 = 0;
int event_scaler4 = 1; // offfset to have the request after command
int event_scaler5 = 1; // offfset to have the request after command


ISR(TIMER1_COMPA_vect) {
  sei();
  timer_count = (timer_count + 1) % 6; // reset counter for the number of events

  if (timer_count == 0 ) {
    event_scaler0 = (event_scaler0 + 1) % TEMP_RATIO;
    if (event_scaler0 == 0) {
      isBodyTempAvailable = requestTemp(BodyTempAdd); timeThis(String(timer_count) + ":request body temp:");
      isEnvTempAvailable = requestTemp(EnvTempAdd); timeThis(String(timer_count) + ":request env temp:");
    }
  }
  
  if (timer_count == 1 ) {
    event_scaler1 = (event_scaler1 + 1) % IMP_RATIO;
    if (event_scaler0 == 0) {
      requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles); timeThis(String(timer_count) + ":Imp Requested:");
    }
  }
  
  if (timer_count == 2 ) {
    getPAT();
  }
  
  if (timer_count == 3 ) {
    getPPG();
  }

  if (timer_count == 4 ) {
    event_scaler4 = (event_scaler4 + 1) % IMP_RATIO;
    if (event_scaler4 == 0) {
      if (checkAD5933()) {
        waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
        timeThis(String(timer_count) + ":idle imp 1:");
        printImp(IMP_REAL, IMP_IMG); //Getting the sample from AD5933 and serial printing
        timeThis("5:reading imp 1:");
      }
    }
  }
  
  if (timer_count == 5 ) {
    event_scaler5 = (event_scaler5 + 1) % TEMP_RATIO;
    if (event_scaler5 == 0) {
      if ( isBodyTempAvailable) {
        temperatureC = getTemp(BodyTempAdd); timeThis(String(timer_count) + ":received body temp:");
        SerialPlot(BODY_TEMP_CHAR, 100 * temperatureC);
        isBodyTempAvailable = false;
      }
      if ( isEnvTempAvailable) {
        temperatureC = getTemp(EnvTempAdd); timeThis("15:received body temp:");
        SerialPlot(ENV_TEMP_CHAR, 100 * temperatureC);
        isEnvTempAvailable = false;
      }
    }

  }


}


int getPPG() {
  // GET ppg values
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  timeThis(String(timer_count) + "read PPG:");
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  SerialPlot(RED_CHAR, RED );
  SerialPlot(IR_CHAR, IR );
  timeThis(String(timer_count) + "communicate PPG:");
  return 0;
}
int getPAT() {
  //  GET Pressure
  isAvailablePAT = writeRegister(ADS1115_ADDRESS, ADS1115_REG_POINTER_CONFIG, 0b1100011110000011); //128sps 0b1100011110000011 == ±1.024 V     0b1100010110000011 == = ±2.048 V (default)
  timeThis("PAT ADC set:");
  delayMicroseconds(1000);
  if (isAvailablePAT) {
    timeThis("check PAT status:");
    PAT = readRegister(ADS1115_ADDRESS, ADS1015_REG_POINTER_CONVERT); timeThis("21:read PAT reg:");
    SerialPlot(PAT_CHAR , PAT); timeThis("communicate PAT :");
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
