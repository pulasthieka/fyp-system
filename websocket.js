var express = require('express');
var app = require('express')();
var path = require('path');
var http = require('http').Server(app);
var bodyParser = require('body-parser');
var mongoose = require('mongoose');
var cors = require('cors');

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
    console.log(msg);
    saveEvent(msg);
  });
  client.on('db', (msg) => {
    console.log(msg);
    // msg = 'DinithiHemakumara';
    socket.emit(collectionName, 'connected to DB');
    createDocsIfNotExists(msg);
    collectionName = msg;
    changeStream = openChangeStream(msg);
    changeStream.on('change', (change) => {
      // console.log(change.updateDescription.updatedFields);
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
  res.sendFile(__dirname + '/graph.html');
});
app.post('/save', function (req, res) {
  console.log(req.body);
});

const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8011 });

wss.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  console.log(ip);
  lastTime = 0;
  ws.on('message', function incoming(message) {
    let msg;
    try {
      msg = JSON.parse(message);
      processTransmission(msg);
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
  console.log('done');
}, 1);

var prefixes = ['Pressure', 'SPO2', 'Temperature', 'Bioimpedance']; // PAT,ECG,Temp,SpO2,BioZ

function openChangeStream(collection) {
  const taskCollection = db.collection(collection);
  const changeStream = taskCollection.watch();
  return changeStream;
}

function processTransmission(req) {
  let Data_From_NodeMCU = req.hello;
  let S_data_server = req.SData;
  let T_data_server = req.TData;
  let B_data_server = req.BData;
  let E_data_server = req.EData;
  //   nDate = Date.now();

  console.log(req);

  //["P","E","T","S","B"]
  data = [Data_From_NodeMCU, E_data_server, T_data_server, S_data_server, B_data_server];
  time = [req.Ptime, req.Etime, req.Ttime, req.Stime, req.Btime];
  //////////////Sending Data\\\\\\\\\\\\\\\\

  for (var i = 0; i < prefixes.length - 1; i++) {
    docname = collectionName + prefixes[i];
    if (data[i].length != 0) {
      // var field = prefixes[i];
      // var time = prefixes[i];
      const timeStamps = time[i];
      lastTime = time[i][time[i].length - 1];
      const processed_data = data[i];
      //pusing to database one by one
      q.push({ processed_data: processed_data, docname: prefixes[i], field: 'values', time: timeStamps });
    }
  }
}
var lastTime = 0;
async function saveEvent(name) {
  db.collection(collectionName).updateOne({ _id: 'events' }, { $push: { [name]: lastTime } }, { upsert: true }, (err) => {
    if (err) console.log('DB error:', err);
  });
  console.log('Saved');
}
async function createDocsIfNotExists(collectionName) {
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
