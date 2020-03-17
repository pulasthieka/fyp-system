//Require the express module and other modules
const express = require('express'),
    bodyParser = require('body-parser'),
    cors = require('cors'),
    mongoose = require('mongoose');

config = {
    DB: 'mongodb://localhost:27017/ng7crud'
 };
// connect to database
mongoose.Promise = global.Promise;
mongoose.connect(config.DB, { useNewUrlParser: true }).then(
  () => {console.log('Database is connected') },
  err => { console.log('Can not connect to the database'+ err)}
);

const sessionRoute = require('./routes/session.route')

//create a new express application
const app = express()
app.use(bodyParser.json());
app.use(cors({origin:'http://localhost:4200'}));
app.use('/session', sessionRoute);
const port = process.env.PORT || 500;

const server = app.listen(port,function(){
    console.log('Listening on port ' + port);
});
// app.listen(port)

// // require the socket.io module
// const io = require('socket.io');

// const socket = io(http);
// //create an event listener

// //To listen to messages
// socket.on('connection', (socket)=>{
// console.log('user connected');
// socket.on("disconnect", ()=>{
//     console.log("Disconnected")
// })
// });

