#include <Wire.h>
const int BAUD_RATE = 115200;

const int MAX30100_ADDRESS = 0x57;
const int MAX30100_FIFO_DATA = 0x05;
const int MAX30100_CTRL_REG = 0x06;
const int MAX30100_SPO2_CONFIG = 0x07;
const int MAX30100_LED_CONFIG = 0x09;


void setup() {
  Wire.begin();
  Serial.begin(BAUD_RATE);
  debug("setup starting");
  setup_ppg();
  //delay(10); //Powerup time max
  debug("setup done");
}

void loop() {

  if (!is_available_ppg()) {
    setup_ppg();
  }
  String ppg_data = read_ppg();
  Serial.println(ppg_data);

}

bool transmitData(int deviceAddress, int subAddress, int value, bool transmissionStop) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(subAddress);
  if (value != -1) {
    Wire.write(value);
    return (Wire.endTransmission() == 0);
  }
  return (Wire.endTransmission(transmissionStop) == 0);
}

int readData(uint8_t deviceAddress, int count)
{
  Wire.requestFrom(deviceAddress, count);
  int i2c_data = Wire.read();  // Read bytes from slave register address
  return i2c_data;
}

bool setup_ppg() {
  bool set_control = transmitData(MAX30100_ADDRESS, MAX30100_CTRL_REG, 0x03, true); // SPO2 enabled
  bool set_led = transmitData(MAX30100_ADDRESS, MAX30100_LED_CONFIG, 0b01011000, true);// RED LED current 0b1110111
  bool set_spo2 = transmitData(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, 0b01000111, true); //100sps 1600us pulse
  return (set_control && set_led && set_spo2);
}

bool is_available_ppg() {
  if (transmitData(MAX30100_ADDRESS, MAX30100_CTRL_REG, -1, false)) {
    int ctrl_reg = readData(MAX30100_ADDRESS, 1);
    debug("control", ctrl_reg);
    return (ctrl_reg == 0x03);
  }
  return false;
}

String read_ppg() {
  if (transmitData(MAX30100_ADDRESS, MAX30100_FIFO_DATA, -1, false)) {
    Wire.requestFrom(MAX30100_ADDRESS, 4);
    int b1 = Wire.read();
    int b2 = Wire.read();
    int b3 = Wire.read();
    int b4 = Wire.read();
    debug("byte1", b1);
    debug("byte2", b2);
    long ir = (b1 << 8) | b2;
    long red = (b3 << 8) | b4;
    return "I" + String(ir) + "X" + "R" + String(red) + "X";
  }
  return "*";
}
