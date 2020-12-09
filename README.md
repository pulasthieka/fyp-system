# FYP Web Application

## Prerequisites

The following packages have to be installed.

1. [MongoDB](https://docs.mongodb.com/manual/installation/),
2. NodeJS,

## First Time install

#### Setup Database

1. Create a new database
2. Setup replica set for the new database
   - Start mongodb `mongodb --dbpath "path-to-database"`
   - Open a new terminal and start a mongo shell `monogo`
   - Type `rs.initiate()`

#### Setup Web-app

1. Install all packages with `npm install`
2. Point the server to database
   - Change config parameter `config = { DB: "mongodb://localhost:27017/dbname?replicaSet=rs"};`

## Startup Procedure

Run the below commands in seperate terminals

1. Start database with replicaset
   `mongod --dbpath "path-to-database" --replSet "rs"`

2. Start node server `nodemon server.js` or `node server.js`

3. Open `index.html` in a browser

## Navigating the graphs

- Click and drag to zoom
- Click and drag to create a negative area to switch to horizontal / vertical zoom
- Mousewheel doesn't work

You can also use the buttons above the graph to navigate the graph.

## Further help

#### Setting up ESP32

0. Install ESP32 core for ArduinoIDE
1. Navigate to C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\VIRSION\cores\esp32
2. Open HardwareSerial.cpp
3. Change line 47 to "rxPin = 2;"
4. Change line 48 to "txPin = 4;"
5. Restart Arduino IDE

#### Testing end to end system

1. Connect your laptop to a Wi-Fi network
2. Run "ipconfig" on CMD and get your laptop's IP address
3. Update line 24 and 25 of ESP32.ino with your WiFI SSID and password
4. Update line 27 of ESP32.ino with the IP address you obtained from step 2
5. Program [ESP32.ino](Hardware\ESP32\ESP323.ino) to ESP32
6. Program [ATTiny_1.ino](Hardware\ATTiny_1\ATTiny_1.ino) to an ATTiny and connect it's pin 3 to D2 of ESP32
7. Program [ATTiny_2.ino](Hardware\ATTiny_2\ATTiny_2.ino) to an ATTiny and connect it's pin 3 to RX2 of ESP32

   <img src="images\ESP32-Pinout.jpg">

8. Run [server.js](server.js) and mongoDB(with replica sets)
   `mongod --dbpath "path-to-database" --replSet "rs"`
9. Go to port 90 of your localhost to see data plots using index.html

note - supporting js files, index.html and server.js must be in the same folder
