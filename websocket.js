var express = require('express');
var app = require('express')();
var path = require('path');
var http = require('http').Server(app);
var bodyParser = require('body-parser');
var mongoose = require('mongoose');
var cors = require('cors');

var times = {};
//mongoose.connect('mongodb://localhost/testdb');
config = {
  DB: 'mongodb://localhost:27017/tryone?replicaSet=rs',
};
mongoose.connect(config.DB, { useNewUrlParser: true, useFindAndModify: false }).then(
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
    //console.log(msg);
    saveEvent(msg);
  });
  client.on('db', (msg) => {
    //console.log(msg);
    // msg = 'DinithiHemakumara';
    socket.emit(collectionName, 'connected to DB');
    createDocsIfNotExists(msg, prefixes1);
    // createDocsIfNotExists(msg, prefixes2);
    collectionName = msg;
    changeStream = openChangeStream(msg);
    changeStream.on('change', (change) => {
      //console.log(change.updateDescription.updatedFields);
      // {type:change.documentKey._id, data:change.updateDescription.updatedValues}
      if (change.operationType === 'update') {
        socket.emit(collectionName, { type: change.documentKey._id, data: change.updateDescription.updatedFields });
      }
    });
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
  server.listen(90, () => {
    console.log('listening on *:90');
  });
});
app.use(express.static(path.join(__dirname, 'public')));
app.get('/', function (req, res) {
  res.sendFile(__dirname + '/plots_page.html');
});
app.post('/save', function (req, res) {
  console.log(req.body);
});

const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8011 });
const wss2 = new WebSocket.Server({ port: 8012 });

wss.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  console.log('Expremental', ip);
  lastTime = 0;
  ws.on('message', function incoming(message) {
    let msg;
    try {
      msg = JSON.parse(message);
      processTransmission(msg, q, prefixes1);
    } catch {
      msg = message;
      console.log(msg);
    }
  });

  ws.send('acknowledge connection');
});

wss2.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  // console.log('Control',ip);
  // lastTime2 = 0;
  ws.on('message', function incoming(message) {
    let msg;
    //console.log(message);
    try {
      msg = JSON.parse(message);
      processTransmission(msg, q2, prefixes2);
    } catch {
      msg = message;
      console.log(msg);
    }
  });

  ws.send('acknowledge connection');
});

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
//npm install async
const queue = require('async/queue');
// var dbObject;
// if (collectionName) {
//     dbObject = db.collection(collectionName); // find by uid
// }
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
const q2 = queue(function (task, cb) {
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
  // console.log('done');
}, 1);

var prefixes1 = ['PData', 'SData', 'TData', 'BData', 'EData','PCData', 'SCData', 'TCData', 'BCData' ]; // PAT,ECG,Temp,SpO2,BioZ
var prefixes2 = ['PressureC', 'SPO2C', 'TemperatureC', 'BioimpedanceC', 'EnvironmentC'];
function openChangeStream(collection) {
  const taskCollection = db.collection(collection);
  const changeStream = taskCollection.watch();
  return changeStream;
}

function processTransmission(req, que) {
  //console.log('Process Transmission', req);
  //////////////Sending Data\\\\\\\\\\\\\\\\
  Object.keys(req).forEach((key) => {
    docname = collectionName + key.toString();
    if (req[key].length != 0) {
      const processed_data = req[key];
      const d = new Date().getTime();
      var i;
      var time = [];
      for (i = 0; i < req[key].length; i++) {
        time.push(d.toString + (i/10000).toString);
      }
      //pusing to database one by one
      que.push({ processed_data: processed_data, docname: key.toString(), field: 'values', time: time });
    }
  });
}
var lastTime = 0;
// var lastTime2 = 0;
async function saveEvent(name) {
  db.collection(collectionName).updateOne({ _id: 'events' }, { $push: { [name]: lastTime } }, { upsert: true }, (err) => {
    if (err) console.log('DB error:', err);
  });
  console.log('Saved');
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
        console.log(result);
        // do something with the document
      })
    );
  });

  await Promise.all(promises);
}
