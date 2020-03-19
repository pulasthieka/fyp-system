const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const sessionRoute = require('./routes/session.route');
const cors = require('cors');
const app = express();

// app.use(cors({origin:'http://localhost:4200'}));
app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "http://localhost:4200");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  res.header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  res.header("Access-Control-Allow-Credentials", "true");
  next();
});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use('/session', sessionRoute);

// mongoose.connect('mongodb://localhost/ng7crud?replicaSet=rs');
config = {
    DB: 'mongodb://localhost:27017/ng7crud?replicaSet=rs'
 };
mongoose.connect(config.DB, { useNewUrlParser: true }).then(
    () => {console.log('Database is connected') },
    err => { console.log('Can not connect to the database'+ err)}
  );

const db = mongoose.connection;

db.on('error', console.error.bind(console, 'Connection Error:'));


  // require the socket.io module
const io = require('socket.io');

var server = require('http').createServer(app);
const socket = io(server);
//create an event listener

//To listen to messages
socket.on('connection', (client)=>{
    console.log('user connected');
    socket.emit("server",["ewgdsfgsd"])
    socket.on("disconnect", ()=>{
    console.log("Disconnected")
    })
});


// 
db.once('open', () => {
    server.listen(9000, () => {
      console.log('Node server running on port 9000');
    });
  
  const taskCollection = db.collection('sessions');
  const changeStream = taskCollection.watch();
    
  changeStream.on('change', (change) => {
    console.log(change);
    if(change.operationType === 'update') {
    socket.emit('server',change.updateDescription)
    }
  });
});


