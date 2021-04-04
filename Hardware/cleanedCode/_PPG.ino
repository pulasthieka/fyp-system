
uint8_t data_len = 8;
uint8_t DataPacketHeader[15];
volatile  long IRR, REDD;
unsigned long previousMillis = 0;

bool PPG_TRANSMISSION_STOP = false;

void PPGbegin() {
  transmitData(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, 0x03, true); // SPO2 enabled
  transmitData(MAX30100_ADDRESS, MAX30100_LED_CONFIG, 0b1110111, true);// RED LED current
  transmitData(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, 0b01000111, true); //100sps 1600us pulse
}

bool checkStatusPpg() {
  transmitData(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, -1, false);
  Serial.println("check pppg status");
  uint8_t dest2 = readData(MAX30100_ADDRESS, 1);
  Serial.println("All read");
  Serial.println(dest2);
  Serial.print("this fails");
  return (dest2 == 0x03);
}

void TakePPG() {
  while (micros() - previousMillis < 10100) { //comment
    delayMicroseconds(1);
  }
  previousMillis = micros();
  uint8_t* temp;  // Temporary buffer for read values
  temp = readDataPpg(4);  // Read four times from the FIFO
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  Serial.print('S' + String(RED) + 'X');
}

uint8_t* readDataPpg( int count)
{ Serial.println("ppg function");
  uint8_t* dest;
  transmitData(MAX30100_ADDRESS, MAX30100_FIFO_DATA, -1, false);
  Serial.println("ppg trasmission");
  dest = readData(MAX30100_ADDRESS,1);
  Serial.println("ppg read");
  return dest;
}
