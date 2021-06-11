# FYP System

## Arduino Installation

Look at https://www.arduino.cc/en/guide/linux

```bash
tar -xvf arduino-1.8.15-linux64.tar.xz
cd arduino-1.8.15/
sudo ./install.sh
```

Adding ESP32 boards

1. Add https://dl.espressif.com/dl/package\_esp32\_index.json to additional board manager URLs
2. Install **esp32** by Expressif Systems by going to `Tools>Boards>Boards Manager`

Open [CombinedOpt.ino](./CombinedOpt/CombinedOpt.ino) - Atmega Code. This has no external libraries and should compile with no issues.

## Install dependancies for ESP32

1. WebSockets by Markus Sattler (https://github.com/Links2004/arduinoWebSockets)
2. Adafruit SSD1306 libary. This also installs the dependent libraries. (https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED)
   1. Adafruit GFX
   2. Adafruit Bus IO

## Common Issues

Issue with compiling for ESP32 on Ubuntu. `pyserial: module not found`.

```gnome
sudo apt install python-is-python3
sudo apt install python3-pip
pip install pyserial
```

Serial port permissions

```gnome
ls -l /dev/ttyUSB0
sudo usermod -a -G dialout <username>
```

Then log out and log in

## Programming ESP32

1. Plug in your ESP32, select the correct COM PORT and select the board as `DOIT ESP32 DEVKIT V1`

> **WARNING :** This is designed to work alongside ATmega328P programmed with [CombinedOpt.ino](./CombinedOpt/CombinedOpt.ino)

2. Connect your laptop to a Wi-Fi network.
3. Run `ipconfig` (or `ifconfig`) on the command prompt and get your laptop's IP address
4. Update the following lines of [esp32-websocket.ino](./esp32-websocket/esp32-websocket.ino) with your WiFI SSID, password and IP address.
   ```c++
   const char* ssid = "wifi"; // update wifi SSID
   const char* password = "pSW"; // enter password
   const char* ip = "192.168.1.8"; // enter the IP address
   ```
5. Program [esp32-websocket.ino](./esp32-websocket/esp32-websocket.ino) to ESP32. Select the board eg: `DOIT ESP32 DEVKIT V1` (shown below). Select the port eg: COM4.

   <img src="..\images\ESP32-Pinout.jpg">

## Programming ATMEGA

@TODO
