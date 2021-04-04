//#define START_FREQ  (10000)
//#define FREQ_INCR   (10000)
//#define NUM_INCR    (0)
//#define REF_RESIST  (1000)
//
//double gain[NUM_INCR + 1];
//int phase[NUM_INCR + 1];
//
//bool IMP_TRANSMISSION_STOP = true;
//
//int device_address = 0x0D;
//float fivekGF = 22755384.527623348;
//unsigned int k = 0b111111111111111;
//long out_cycles = 15;
//long img = 0;
//long real = 0;
//
//void CheckStatusImp(int deviceAddress) //Ckeck status of circuit(page 26)
//{
//  transmitData(deviceAddress, 0x8F, -1, IMP_TRANSMISSION_STOP); 
//  Wire.requestFrom(device_address, 1);
//  Serial.print("Status = ");
//  Serial.println(Wire.read(), BIN);
//}
//
//unsigned int negcor( unsigned int n) {
//  if (n > k) {
//    return ~n + 1;
//  }
//  else {
//    return n;
//  }
//}
//
//void getimp(int freq1, int freq2, int freq3,char prefix)
//{
//  unsigned int imfive;
//  unsigned int refive;
//  unsigned int imten;
//  unsigned int reten;
//  long imp5k;
//  long imp10k;
//  
//  transmitData(device_address, 0x80, 0b0, true); //setting control reg
//  transmitData(device_address, 0x81, 0b10000, true); //setting control reg (reset=1, using internal clock)
//  
//  //start frequency to 5k
//  transmitData(device_address, 0x82, freq1, true);
//  transmitData(device_address, 0x83, freq2, true);
//  transmitData(device_address, 0x84, freq3, true);
//  
//  //incriment to 0
//  transmitData(device_address, 0x85, 0x00, true);
//  transmitData(device_address, 0x86, 0x00, true);
//  transmitData(device_address, 0x87, 0x00, true);
//  
//  //number of incriments to 0
//  transmitData(device_address, 0x88, 0, true);
//  transmitData(device_address, 0x89, 1, true);
//  
//  //settling cycles
//  transmitData(device_address, 0x8A, 0, true);
//  transmitData(device_address, 0x8B, 0b0001, true);
//  
//  //Taking measurement
//  transmitData(device_address, 0x80, 0b10110001, true); //standby
//  transmitData(device_address, 0x81, 0b00010000, true); //reset
//  transmitData(device_address, 0x80, 0b00010001, true); //initialize
//  
//  // delayMicroseconds(2); //wait for 2ms to settle down
//  // transmitData(device_address, 0x80, 0b00100001, true); //start swipe
//  // while ((readData(device_address, 0x8F, IMP_TRANSMISSION_STOP) & 0b00000010) != 2 ) {
//  //   delayMicroseconds(1);
//  // }
//  
//  // //  delayMicroseconds(10);
//  // real = readData(device_address, 0x94, IMP_TRANSMISSION_STOP) << 7;
//  // real = real << 1 | readData(device_address, 0x95, IMP_TRANSMISSION_STOP);
//  // img = readData(device_address, 0x96, IMP_TRANSMISSION_STOP) << 7;
//  // img = img << 1 | readData(device_address, 0x97, IMP_TRANSMISSION_STOP);
//  // Serial.print(prefix+String(real)+'.'+String(img)+'X');
//}
