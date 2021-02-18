#include <Wire.h>

#define MAX30100_ADDRESS        0x57
#define MAX30100_INT_STATUS     0x00  // Which interrupts are tripped
#define MAX30100_INT_ENABLE     0x01  // Which interrupts are active
#define MAX30100_FIFO_WR_PTR    0x02  // Where data is being written
#define MAX30100_OVRFLOW_CTR    0x03  // Number of lost samples
#define MAX30100_FIFO_RD_PTR    0x04  // Where to read from
#define MAX30100_FIFO_DATA      0x05  // Ouput data buffer
#define MAX30100_MODE_CONFIG    0x06  // Control register
#define MAX30100_SPO2_CONFIG    0x07  // Oximetry settings
#define MAX30100_LED_CONFIG     0x09  // Pulse width and power of LEDs
#define MAX30100_TEMP_INTG      0x16  // Temperature value, whole number
#define MAX30100_TEMP_FRAC      0x17  // Temperature value, fraction
#define MAX30100_REV_ID         0xFE  // Part revision
#define MAX30100_PART_ID        0xFF  // Part ID, normally 0x11

uint8_t data_len = 8;
uint8_t DataPacketHeader[15];
volatile  long IRR, REDD;
unsigned long previousMillis = 0;



long tt = 0;

int device_address = 0x0D;
float fivekGF = 22755384.527623348;
unsigned int k = 0b111111111111111;
long out_cycles = 15;
long img = 0;
long real = 0;
unsigned long StartTime ;
unsigned long CurrentTime;
unsigned long ElapsedTime ;
void setup()
{
  pinMode(A3, OUTPUT);
  analogWrite(A3, 0);
    pinMode(A2, OUTPUT);
  analogWrite(A2, 0);
  Wire.begin();
//  Wire.setClock(400000);
  Serial.begin(2000000);
  while (!Serial);             // Wait for serial monitor
//  Serial.println("Checking AD5933");
  Wire.beginTransmission(device_address);
  //Checking ad5933
  if (!Wire.endTransmission()) {
//    Serial.println("AD5933 found");
    analogWrite(A3, 150);
    delay(100);
    analogWrite(A3, 0);
  }
  else {
//    Serial.println("Error - AD5933 not found");
    analogWrite(A3, 150);
    delay(300);
    analogWrite(A3, 0);
    delay(300);
    analogWrite(A3, 150);
    delay(300);
    analogWrite(A3, 0);
  }

  //checking max30100
  Wire.beginTransmission(MAX30100_ADDRESS);
  if (!Wire.endTransmission()) {
    begin();
//    Serial.println("Max30100 found");
    analogWrite(A2, 150);
    delay(100);
    analogWrite(A2, 0);
  }
  else {
//    Serial.println("Error - MAX30100 not found");
    analogWrite(A2, 150);

    delay(100);
    analogWrite(A2, 0);
    delay(100);
    analogWrite(A2, 150);
    delay(100);
    analogWrite(A2, 0);
  }
}


void loop()
{ //SpO2

//  uint8_t temp[4] = {0};  // Temporary buffer for read values
//  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
//  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
//  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
//  //  Serial.println(RED); //Uncomment to use RED value
//  //  Serial.print("\t");
//  Serial.print('I'+String(IR)+'X'+'R'+String(RED)+'X');

  TakePPG();
  getimp(0x18, 0x6A, 0x73,'R'); //50k//getimp(0x02, 0x71, 0x0B,'A'); //5k
  TakePPG();
  getimp(0x18, 0x6A, 0x73,'R'); //50k //  getimp(0x04, 0xE2, 0x17,'B'); //10k   
  TakePPG();
  getimp(0x18, 0x6A, 0x73,'R'); //50k
//  Serial.println(micros() - previousMillis);
}


void TakePPG(void){
  while (micros() - previousMillis < 10100) { //comment
    delayMicroseconds(1);
  }
  previousMillis = micros();
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
  //  Serial.println(RED); //Uncomment to use RED value
  //  Serial.print("\t");
  Serial.print('S'+String(RED)+'X');
  
  }


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

///////////////////////////////////////////////SpO2 Max30100

void begin(void) {
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, 0x03); // SPO2 enabled
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG, 0b01011000);//0b01011000);  // RED LED and IR 11mA
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, 0b01000111); //100sps 1600us pulse
}


// Wire.h read and write protocols
void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

uint8_t I2CreadByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data; // `data` will store the register data
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                   // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

uint8_t I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  // Next send the register to be read. OR with 0x80 to indicate multi-read.
  Wire.write(subAddress);     // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count);  // Read bytes from slave register address
  while (Wire.available())
  {
    dest[i++] = Wire.read(); // Put read results in the Rx buffer
  }
}