int SerialPlot(String measurement, int value) {
    Serial.println(measurement + ": "); Serial.print(String(value)); Serial.print("  ");
//  Serial.print(measurement + String(value) + "X");
  return 0;
}

bool transmitData(int deviceAddress, int subAddress, int value, bool transmissionStop) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(subAddress);
  if (value != -1){
    Wire.write(value);
    return (Wire.endTransmission() == 0);
  }
  return (Wire.endTransmission(transmissionStop) == 0);
}

uint8_t readData(uint8_t* result, uint8_t deviceAddress, int count)
{
  Serial.println("read data");
//  uint8_t data2[count];
  // transmitData(deviceAddress, subAddress, -1, transmissionStop);  
  Wire.requestFrom(deviceAddress, count); 
   uint8_t data2 = Wire.read();  // Read bytes from slave register address
//  for (int i =0; i > count; i++ )
//  {
//    data2[i] = Wire.read();
//  }      
  // Wire.requestFrom(deviceAddress, (uint8_t) 1);  
  Serial.print("returning read data");
  return data2;                            
}
