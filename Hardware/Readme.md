# Quick Start

## Setting up ESP32

1. Install ESP32 core for Arduino IDE (for versions prior to v1.8.13)
2. Install the required libraries
   - HardwareSerial by Adrian McEwen
   - Adafruit SSD1306 by Adafruit
   - Other libraries are either already available or installed along with others.
3. Plug in your ESP32, select the correct COM PORT and select the board as `DOIT ESP32 DEVKIT V1`
4. Compile and upload code

**WARNING :** This is designed to work alongside ATmega328P programmed with [ATmega328-ppg&bio-imp.ino](/ATmega328-ppg&bio-imp/ATmega328-ppg&bio-imp.ino) and [ATmega328-temperature.ino](/ATmega328-temperature/ATmega328-temperature.ino)

#### Testing end to end system

1. Connect your laptop to a Wi-Fi network
2. Run "ipconfig" on CMD and get your laptop's IP address
3. Update line 24 and 25 of ESP32.ino with your WiFI SSID and password
4. Update line 27 of ESP32.ino with the IP address you obtained from step 2
5. Program [esp32-v2.ino](esp32-v2\esp32-v2.ino) to ESP32
   <img src="..\images\ESP32-Pinout.jpg">
6. Program [ATmega328-ppg&bio-imp.ino](/ATmega328-ppg&bio-imp/ATmega328-ppg&bio-imp.ino) to an ATmega328P and connect it's pin 3 to D2 of ESP32
7. Program [ATmega328-temperature.ino](/ATmega328-temperature/ATmega328-temperature.ino) to an ATmega328P and connect it's pin 3 to RX2 of ESP32
8. Connect Sensors to I2C ports and provide power to all processors
   <img src="..\images\atmega328p-pinout.gif">

9. Run [server.js](server.js) and mongoDB(with replica sets)
   `mongod --dbpath "path-to-database" --replSet "rs"`
10. Go to port 90 of your localhost to see data plots using `index.html`. **Note** - supporting js files, index.html and server.js must be in the same folder
