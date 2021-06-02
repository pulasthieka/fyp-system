/*
  Uses timer1 interrupts to control blink
*/
#include <Wire.h>

#define MAX30100_ADDRESS        0x57
#define MAX30100_FIFO_DATA      0x05  // Ouput data buffer
#define MAX30100_MODE_CONFIG    0x06  // Control register
#define MAX30100_SPO2_CONFIG    0x07  // Oximetry settings
#define MAX30100_LED_CONFIG     0x09  // Pulse width and power of LEDs

int SerialPlot(String measurement, long value) {
  Serial.print(measurement + ": "); Serial.print(value); Serial.println("  ");
  //  Serial.print(measurement + String(value) + "X");
  return 0;
}

int debug(String message){
  Serial.println("DEBUG : " + message);
  return 0;
}
