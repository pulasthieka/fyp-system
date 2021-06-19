var express = require('express');
const classes = require('./classes');
var app = require('express')();
var path = require('path');
var http = require('http').Server(app);
const queue = require('async/queue');
const mongoose = require('mongoose');
var bodyParser = require('body-parser');
var cors = require('cors');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
var times = {};
//mongoose.connect('mongodb://localhost/testdb');
config = {
  DB: 'mongodb://localhost:27017/tryone',
};
mongoose.connect(config.DB, { useNewUrlParser: true, useFindAndModify: false, useUnifiedTopology: true }).then(
  () => {
    console.log('Database is connected');
  },
  (err) => {
    console.log('Can not connect to the database' + err);
  }
);

const db = mongoose.connection;

db.on('error', console.error.bind(console, 'Connection Error:'));

var io = require('socket.io');
var server = require('http').createServer(app);

var socket = io(server);
let collectionName = 'default';
socket.on('connection', (client) => {
  let changeStream;
  client.on('event', (msg) => {
    saveEvent(msg);
  });

  client.on('clear', () => {
    clearPlots();
  });

  client.on('db', (msg) => {
    socket.emit(collectionName, 'connected to DB');
    createDocsIfNotExists(msg, prefixes1);
    collectionName = msg;
    // changeStream = openChangeStream(msg);
    // changeStream.on('change', (change) => {
    //   if (change.operationType === 'update') {
    //     socket.emit(collectionName, { type: change.documentKey._id, data: change.updateDescription.updatedFields });
    //   }
    // });
  });
  console.log('user connected');
  client.on('disconnect', () => {
    console.log('user disconnected');
    if (changeStream) {
      changeStream.close();
    }
  });
});

db.once('open', () => {
  server.listen(9090, () => {
    console.log('listening on *:90');
  });
});
app.use(express.static(path.join(__dirname, 'public')));
//adding the form page to collect patient data
app.get('/', function (req, res) {
  // res.sendFile(__dirname + '/form.html');
  res.sendFile(__dirname + '/plots_page.html');
});
app.get('/plots', function (req, res) {
  res.sendFile(__dirname + '/plots_page.html');
});
app.post('/save', function (req, res) {
  console.log(req.body);
});
//to save patient data entered in the form to mongodb
app.post('/patient-data', function (req, res) {
  //console.log('req', req.body);
  var myData = createNewUser(req.body);
  console.log('mydata', myData);
  myData.save(function (err, results) {
    console.log(err);
    let uid = results._id;
    //console.log(results);
    res.redirect('/plots?id=' + results.fname + results.lname);
  });
});
//setting collectionName to patient name
function createNewUser(data) {
  const nameSchema = new mongoose.Schema(
    {
      fname: String,
      lname: String,
      age: Number,
      height: Number,
      weight: Number,
      gender: String,
      phone: Number,
      condition: String,
      comment: String,
    },
    { collectionName: data.fname + data.lname }
  );
  const User = mongoose.model('User', nameSchema);
  return new User(data);
}
async function getName(uid) {
  let document = await User.findById(uid);
  console.log(document);
  return document.fname + ' ' + document.lname;
}

const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8011 });
const wss2 = new WebSocket.Server({ port: 8012 });

wss.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  console.log('New connection from', ip);
  let messengers = {}; // create front end messengers
  prefixes1.forEach((signal) => {
    messengers[signal] = new classes.Messenger(signal);
  });
  ws.on('message', function incoming(message) {
    let msg;
    try {
      msg = JSON.parse(message);

      Object.keys(msg).forEach((key) => {
        // sending to the frontend
        if (msg[key].length != 0) {
          // avoid empty messages
          let frontendMsg = messengers[key].send(msg[key]);
          socket.emit('signals', frontendMsg);
        }
      });
      processTransmission(msg, q);
    } catch (err) {
      msg = message;
      console.log('processing error', err.message, msg);
    }
  });
  ws.on('close', function informDisconnect() {
    console.log('Device Disconnected');
  });
  ws.send('acknowledge connection');
});

// //npm install async

const q = queue(function (task, cb) {
  // console.log(task);
  db.collection(collectionName).updateOne(
    { _id: task.docname },
    { $push: { [task.field]: { $each: task.processed_data }, ['timeStamps']: { $each: task.time } } },
    // { upsert: true },
    (err) => {
      if (err) console.log('DB error:', err);
      cb();
    }
  );
}, 1);

var prefixes1 = ['PData', 'SData', 'TData', 'BData', 'EData', 'PCData', 'SCData', 'TCData', 'BCData', 'WData', 'WCData', 'YData', 'YCData']; // PAT,ECG,Temp,SpO2,BioZ
function openChangeStream(collection) {
  const taskCollection = db.collection(collection);
  const changeStream = taskCollection.watch();
  return changeStream;
}

function processTransmission(req, que) {
  // console.table(req);
  const d = new Date();
  let timeInt = d.getTime();
  Object.keys(req).forEach((key) => {
    // docname = collectionName + key.toString();
    if (req[key].length != 0) {
      const processed_data = req[key];
      let time = Array(processed_data.length).fill(timeInt);
      //pusing to database one by one
      que.push({ processed_data: processed_data, docname: key.toString(), field: 'values', time: time });
    }
  });
}

async function saveEvent(name) {
  const d = new Date();
  let timeInt = d.getTime();
  db.collection(collectionName).updateOne({ _id: 'events' }, { $push: { [name]: timeInt } }, { upsert: true }, (err) => {
    if (err) console.log('DB error:', err);
  });
  console.log('Saved', name, timeInt, collectionName);
}

async function clearPlots() {
  const model = mongoose.connection.models['Recording'];
  // console.log(model)
  let promises = [];
  prefixes1.forEach((el) => {
    promises.push(
      model.deleteOne({ name: el }, {}, function (error, result) {
        if (error) return;
        // console.log('Patient Document :', result);
      })
    );
  });
  await Promise.all(promises);
  createDocsIfNotExists(collectionName, prefixes1);
  console.log('Collection: ', collectionName, ' reset');
}

async function createDocsIfNotExists(collectionName, prefixes) {
  delete mongoose.connection.models['Recording'];
  const recordingSchema = new mongoose.Schema(
    {
      _id: String,
      timeStamps: [[Number]],
      values: [[Number]],
      name: String,
    },
    { collection: collectionName }
  );
  const Recording = mongoose.model('Recording', recordingSchema);
  var update = { expire: new Date() },
    options = { upsert: true, new: true, setDefaultsOnInsert: true };
  let promises = [];
  prefixes.forEach((el) => {
    promises.push(
      Recording.findOneAndUpdate({ _id: el, name: el }, update, options, function (error, result) {
        if (error) return;
        console.log('Patient Document :', result);
      })
    );
  });

  await Promise.all(promises);
}
