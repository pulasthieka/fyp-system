const classes = require('./classes');
const db = require('./database');

const express = require('express'); //nodejs server
const app = express();
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

const connectionList = []; // array to maintain multiple connections

// SECTION: Frontend Connections
socket.on('connection', (client) => {
  console.log('New connection from', client.handshake.address);
  let dbConnection = false;
  let collectionName;

  // button clicks for events
  client.on('event', (msg) => {
    try {
      const d = new Date();
      const timeInt = d.getTime(); // get time and add to message
      dbConnection.updateEvents(msg, [timeInt]);
    } catch (err) {
      console.log('Cannot save event', err);
    }
  });

  // clear the database
  client.on('clear', () => {
    if (dbConnection) {
      dbConnection.remove(); // empty database & recreate
    } else {
      console.log('Cannot clear, Database unavailable');
    }
  });

  // new connection
  client.on('db', async (msg) => {
    collectionName = !msg ? 'defaultCollection' : msg; // avoid passing null collection names
    dbConnection = new db.Database(collectionName, bioSignals);
    let isConnected = await dbConnection.connect(config.DB + 'fyp'); //specify database name
    console.log('Is connected to DB ?', isConnected);
    if (isConnected) {
      let indx = connectionList.length;
      dbConnection.initiate();
      connectionList.push(dbConnection);
      // connection.on('error', (err) => {
      //   connections = connections.splice(indx, 1);
      // });
      console.log(`${collectionName} connected`);
    }
  });

  client.on('disconnect', () => {
    try {
      dbConnection.disconnect();
      console.log(`${collectionName} disconnected`);
    } catch (err) {
      console.log("Couldn't disconnect", err);
    }
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
      console.table(msg);
      Object.keys(msg).forEach((key) => {
        if (msg[key].length != 0) {
          let frontendMsg = messengers[key].send(msg[key]);
          socket.emit('signals', frontendMsg);
          if (connectionList[0]) {
            let database = connectionList[0]; // @TODO multiple connections
            database.updateSignals(key, frontendMsg.data, frontendMsg.time); // save to db
          }
        }
      });
    } catch (err) {
      console.log('Cannot process transmission', err.message);
      console.table(message);
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
