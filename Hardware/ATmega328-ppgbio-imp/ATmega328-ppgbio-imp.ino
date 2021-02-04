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

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(2000000);
  //setting up sensor
//  Serial.println("SETUP-STARTING");
  begin();
//  Serial.println("SETUP-COMPLETED");
}

void loop() {
//  Serial.println(I2CreadByte(MAX30100_ADDRESS,MAX30100_FIFO_WR_PTR));//-I2CreadByte(MAX30100_ADDRESS,MAX30100_FIFO_RD_PTR));
  while(micros()-previousMillis<10100){
    delayMicroseconds(1);
    }
  previousMillis = micros();
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  long IR = (temp[0] << 8) | temp[1];  // Combine values to get the actual number
  long RED = (temp[2] << 8) | temp[3]; // Combine values to get the actual number
//    Serial.println(RED); //Uncomment to use RED value
Serial.print("R" + String(RED) + 'X');
  //  Serial.print("\t");
  Serial.print("I" + String(IR) + 'X');
//    Serial.println(IR);
}



//void readSensor(void){
//  uint8_t temp[4] = {0};  // Temporary buffer for read values
//  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
//  long IR = (temp[0]<<8) | temp[1];    // Combine values to get the actual number
//  long RED = (temp[2]<<8) | temp[3];   // Combine values to get the actual number
////  Serial.print(RED); //Uncomment to use RED value
//  Serial.println(IR);
//}

void begin(void) {
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, 0x03); // SPO2 enabled
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG, 0b1110111);// RED LED current
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
  Wire.write(subAddress);	                 // Put slave register address in Tx buffer
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
