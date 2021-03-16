#define START_FREQ  (10000)
#define FREQ_INCR   (10000)
#define NUM_INCR    (0)
#define REF_RESIST  (1000)

double gain[NUM_INCR + 1];
int phase[NUM_INCR + 1];

int device_address = 0x0D;
float fivekGF = 22755384.527623348;
unsigned int k = 0b111111111111111;
long out_cycles = 15;
long img = 0;
long real = 0;

void ADwrite(int address, int data)
{
  Wire.beginTransmission(device_address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
byte ADread(int address) //read one byte from address
{
  Wire.beginTransmission(device_address);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(device_address, 1);
  return Wire.read();
}
void CheckStatus() //Ckeck status of circuit(page 26)
{
  Wire.beginTransmission(device_address);
  Wire.write(0x8F);
  Wire.endTransmission();
  Wire.requestFrom(device_address, 1);
  Serial.print("Status = ");
  Serial.println(Wire.read(), BIN);
}

unsigned int negcor( unsigned int n) {
  if (n > k) {
    return ~n + 1;
  }

  else {
    return n;
  }
}

void getimp(int freq1, int freq2, int freq3,char prefix)
{
  unsigned int imfive;
  unsigned int refive;
  unsigned int imten;
  unsigned int reten;
  long imp5k;
  long imp10k;
  ADwrite(0x80, 0b0); //setting control reg
  ADwrite(0x81, 0b10000); //setting control reg (reset=1, using internal clock)
  //start frequency to 5k
  ADwrite(0x82, freq1);
  ADwrite(0x83, freq2);
  ADwrite(0x84, freq3);
  //incriment to 0
  ADwrite(0x85, 0x00);
  ADwrite(0x86, 0x00);
  ADwrite(0x87, 0x00);
  //number of incriments to 0
  ADwrite(0x88, 0);
  ADwrite(0x89, 1);
  //settling cycles
  ADwrite(0x8A, 0);
  ADwrite(0x8B, 0b0001);
  //Taking measurement
  ADwrite(0x80, 0b10110001); //standby
  ADwrite(0x81, 0b00010000); //reset
  ADwrite(0x80, 0b00010001); //initialize
  delayMicroseconds(2); //wait for 2ms to settle down
  ADwrite(0x80, 0b00100001); //start swipe
  while ((ADread(0x8F) & 0b00000010) != 2 ) {
    delayMicroseconds(1);
  }
  //  delayMicroseconds(10);
  real = ADread(0x94) << 7;
  real = real << 1 | ADread(0x95);
  img = ADread(0x96) << 7;
  img = img << 1 | ADread(0x97);
  Serial.print(prefix+String(real)+'.'+String(img)+'X');
  //  Serial.print("\t");
  // while ((ADread(0x8F) & 0b00000010) != 2 ) {delayMicroseconds(1);}
  //   real=ADread(0x94)<<7;
  //  real=real<<1 | ADread(0x95);
  //  img= ADread(0x96)<<7;
  //  img= img<<1 | ADread(0x97);
  //  ADwrite(0x80,0b00110001);
  // Serial.print(String(real));
  // Serial.print("\t");
  // Serial.println(String(img));
  // Serial.println("====");
}
