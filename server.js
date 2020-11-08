var app = require("express")();
var http = require("http").Server(app);
var bodyParser = require("body-parser");
var mongoose = require("mongoose");
var cors = require("cors");

//mongoose.connect('mongodb://localhost/testdb');
config = {
  DB: "mongodb://localhost:27017/tryone?replicaSet=rs",
};
mongoose.connect(config.DB, { useNewUrlParser: true }).then(
  () => {
    console.log("Database is connected");
  },
  (err) => {
    console.log("Can not connect to the database" + err);
  }
);

const db = mongoose.connection;

db.on("error", console.error.bind(console, "Connection Error:"));

var io = require("socket.io");
var server = require("http").createServer(app);

var socket = io(server)

socket.on("connection", (client) => {
  console.log("user connected");
  socket.on("disconnect", () => {
    console.log("user disconnected");
  });
});

db.once("open", () => {
  server.listen(90, () => {
    console.log("listening on *:90");
  });

  const taskCollection = db.collection("tryonecol");
  const changeStream = taskCollection.watch();

  changeStream.on("change", (change) => {
  // console.log(change.updateDescription.updatedFields);
  //console.log(keys(change.updateDescription.updatedFields));
    if (change.operationType === "update") {
      socket.emit("server", change.updateDescription);      
    }
  });
});

app.get("/", function (req, res) {
  res.sendFile(__dirname + "/graph.html");
});
app.get("/socket.io.js", function (req, res) {
  res.sendFile(__dirname + "/socket.io.js");
});
app.get("/jquery-3.5.1.js", function (req, res) {
  res.sendFile(__dirname + "/jquery-3.5.1.js");
});
app.get("/canvasjs.min.js", function (req, res) {
  res.sendFile(__dirname + "/canvasjs.min.js");
});



app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());
//Var for POST
var dbObject = db.collection('tryonecol');
var Data_From_NodeMCU;
var UART_2;
var UART_1;
var nDate;
var prefixes=["P","E","T","S","B"];//PAT,ECG,Temp,SpO2,BioZ
var handel;
var loopbound;
var data;
var readings;
var readings_type;
//POST to save out data in DB
app.post("/saveData", function(req, res){
  //console.log(req.body);  

      Data_From_NodeMCU = req.body.hello;
      UART_2 = req.body.hello2;
      UART_1 = req.body.hello3;                    
      nDate = Date.now(); 
      
      //sending ack  
      //console.log(nDate);  
      //correcting Serial2 buffer overflow
      handel = /[S|B]/.exec(UART_2);
      if (handel){UART_2=UART_2.slice(handel.index);}
      //generating lists
      readings=Data_From_NodeMCU.split(/[P]/).filter (x => x).map(x => Number(x));                     
      ///////////////Binning data\\\\\\\\\\\\\
      //Handling NodeMCU ADC1 data
     
      //["P","E","T","S","B"]
      data=[readings,[],[],[],[]];
      //Handling Serial2      
      readings=UART_2.split(/[S|B]/).filter (x => x).map(x => Number(x)); 
      readings_type = UART_2.split (/[\d|.]+/).filter (x => x);  
      loopbound=readings_type.length;     
      for (var dummy_2=0;dummy_2<loopbound;dummy_2++){
        if (readings_type[dummy_2]=="S"){data[3].push(readings[dummy_2]);}
        else if(readings_type[dummy_2]=="B"){data[4].push(readings[dummy_2]);}
        else{console.log("ERROR");console.log(readings,readings_type,);}
      }
      //Handling Serial 1
      var handel = /[E|T]/.exec(UART_1);
      if (handel){UART_1=UART_1.slice(handel.index);}

      readings=UART_1.split(/[E|T]/).filter (x => x).map(x => Number(x)); 
      readings_type = UART_1.split (/[\d|.]+/).filter (x => x);  
      loopbound=readings_type.length;
      for (var dummy_2=0;dummy_2<loopbound;dummy_2++){
        if (readings_type[dummy_2]=="E"){data[1].push(readings[dummy_2]);}
        else if(readings_type[dummy_2]=="T"){data[2].push(readings[dummy_2]);}
        else{console.log("ERROR");console.log(readings,readings_type,req.body.hello3);}
      }
      
      //////////////Sending Data\\\\\\\\\\\\\\\\
      var dummy;
      loopbound=prefixes.length;
      for (dummy=0;dummy<loopbound;dummy++){
        if (data[dummy].length!=0){
          var field=prefixes[dummy]+".1";
          var time=prefixes[dummy]+".0";
          //var data_cluster = data[dummy];
          // var time_cluster =[];
          // var append;
          // for (append of data[dummy]){
          //   time_cluster.push(nDate);
          // }
          
          dbObject.updateOne(
            {name: "AKILA"}, 
            {'$push': {[field] :{$each: data[dummy]} }},
            function (err) {
              if (err) 
              {
                console.log("DB error:");
                console.log(err);
              }})
              ;   
          // dbObject.updateOne(
          //   {name: "AKILA"}, 
          //   //{'$push': {[time] :{$each: time_cluster} }},
          //   {'$push': {[time] :nDate }},
          //   function (err) {
          //     if (err) 
          //     {
          //       console.log("DB error:");
          //       console.log(err);
          //     }});      
        }

      }  
      res.send("a");
  
	
});  
