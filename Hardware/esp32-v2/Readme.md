# Getting started

1. Install ESP32 core for Arduino IDE (for versions prior to v1.8.13)
2. Install the required libraries
   - HardwareSerial by Adrian McEwen
   - Adafruit SSD1306 by Adafruit
   - Other libraries are either already available or installed along with others.
3. Plug in your ESP32, select the correct COM PORT and select the board as `DOIT ESP32 DEVKIT V1`
4. Compile and upload code

**WARNING :** This is designed to work alongside ATmega328P programmed with [ATmega328-ppg&bio-imp.ino](../ATmega328-ppg&bio-imp/ATmega328-ppg&bio-imp.ino) and [ATmega328-temperature.ino](../ATmega328-temperature/ATmega328-temperature.ino)

## Extending support for other signals

1. Create variable to store signal data
   ```c++
   String NewData = "[";
   ```
2. Add to switch case block. **Add before `default` case**
   ```c++
   void readBuffer(HardwareSerial buffReader){
       ...
       switch (rx) {
       case 'N':
           active = true;
           combineTo = &NewData;
           break;
       ...
   }
   ```
3. Add to the HTTPpayload and add the removeComma statement in `Send_data` function

   ```c++
   void Send_data(WiFiClient cl) {
        ...
        removeComma(NewData);
        ...
        payload = ... + NewData + "] ," ...;
   }

   ```

4. Add to `resetData`
   ```c++
   void resetData(){
       ...
       NewData = "[";
   }
   ```
