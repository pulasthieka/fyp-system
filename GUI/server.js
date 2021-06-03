const classes = require('./classes');
const db = require('./database');

const express = require('express'); //nodejs server
const app = require('express')();
const path = require('path'); // used to deliver static file eg: CSS and JS libraries
const bodyParser = require('body-parser');
const cors = require('cors');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
const server = require('http').createServer(app);

const config = {
  DB: 'mongodb://127.0.0.1:27017/',
  PORT: 9090,
  WS_PORT: 8011,
};

const io = require('socket.io');
const socket = io(server); // websockets connection to gui using socket io
const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: config.WS_PORT }); // socket connection to Hardware

const bioSignals = ['PData', 'SData', 'TData', 'BData', 'WData', 'WCData', 'EData', 'PCData', 'SCData', 'TCData', 'BCData', 'YData', 'YCData']; // PAT,ECG,Temp,SpO2,BioZ

server.listen(config.PORT, () => {
  console.log(`Listening on *:${config.PORT}`);
}); //create local server

const connections = [];

// SECTION: Frontend Connections
socket.on('connection', (client) => {
  let database;
  let collectionName;

  client.on('event', (msg) => {
    if (database) {
      const d = new Date();
      const timeInt = d.getTime();
      // get time and add to message
      database.updateEvents(msg, [timeInt]);
    } else {
      console.log('Cannot save event, Database unavailable');
    }
  });

  client.on('clear', () => {
    if (database) {
      database.remove(); // empty database / recreate
    } else {
      console.log('Cannot clear, Database unavailable');
    }
  });

  client.on('db', (msg) => {
    collectionName = !msg ? 'defaultCollection' : msg; // avoid passing null collection names
    database = new db.Database(collectionName, bioSignals);
    let connection = database.connect(config.DB + 'fyp'); //specify database name
    console.log(connection);
    if (connection) {
      // maintain multiple connections
      let indx = connections.length;
      database.initiate();
      connections.push(database);
      // connection.on('error', (err) => {
      //   connections = connections.splice(indx, 1);
      // });
    }
    console.log(`${collectionName} connected`);
  });

  client.on('disconnect', () => {
    console.log(`${collectionName} disconnected`);
  });
});

//SECTION: connection with Hardware system
wss.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  console.log('HW connection from : ', ip);

  let messengers = {}; // create front end messengers
  bioSignals.forEach((signal) => {
    messengers[signal] = new classes.Messenger(signal);
  });

  ws.on('message', function incoming(message) {
    try {
      let msg = JSON.parse(message);

      Object.keys(msg).forEach((key) => {
        // sending to the frontend
        if (msg[key].length != 0) {
          // avoid empty messages
          let frontendMsg = messengers[key].send(msg[key]);
          socket.emit('signals', frontendMsg);
          if (connections[0]) {
            // writes to first connection
            let database = connections[0];
            database.updateSignals(key, frontendMsg.data, frontendMsg.time); // save to db
          }
        }
      });

      //processTransmission(msg, q, bioSignals);
    } catch (err) {
      console.log(err.message, message);
    }
  });

  ws.on('close', function informDisconnect() {
    console.log(`${ip} disconnected`);
  });
});

// SECTION: HTML pages
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
  // res.sendFile(__dirname + '/form.html');
  res.sendFile(__dirname + '/plots_page.html');
});

app.get('/plots', (req, res) => {
  res.sendFile(__dirname + '/plots_page.html');
});

//to save patient data entered in the form to mongodb
app.post('/patient-data', function (req, res) {
  const document = db.createNewUser(req.body);
  document.save(function (err, results) {
    console.log(err);
    let uid = results._id;
    res.redirect('/plots?id=' + uid);
  });
});
