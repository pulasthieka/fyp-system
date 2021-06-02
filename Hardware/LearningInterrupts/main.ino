//storage variables
boolean toggle1 = 0;
int count = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  debug("starting setting up interrupts");
  cli();//stop interrupts

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 324;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  debug("Setup complete");
}

ISR(TIMER1_COMPA_vect) { //timer1 interrupt 1Hz toggles pin 13 (LED)
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  debug("interrupt hit");
  if (toggle1) {
    digitalWrite(LED_BUILTIN, HIGH);
    toggle1 = 0;
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    toggle1 = 1;
  }
  sei();
  count += 1;
  switch (count % 2) {
    case 0:
      requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles);
      break;
    case 1:
      if (checkAD5933()) {
        requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles);
        waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
        printImp("B", "b"); //Getting the sample from AD5933 and serial printing
        waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
        printImp("B", "b"); //Getting the sample from AD5933 and serial printing
      }
      break;
  }
}


// the loop function runs over and over again forever
void loop() {
}
