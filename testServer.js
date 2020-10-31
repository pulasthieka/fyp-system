const express = require("express");
// const bodyParser = require("body-parser");
const mongoose = require("mongoose");
// const cors = require("cors");
const app = express();

// app.use(cors({origin:'http://localhost:4200'}));
// app.use((req, res, next) => {
//   res.header("Access-Control-Allow-Origin", "http://localhost:4200");
//   res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
//   res.header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
//   res.header("Access-Control-Allow-Credentials", "true");
//   next();
// });

// app.use(bodyParser.json());
// app.use(bodyParser.urlencoded({ extended: true }));

// mongoose.connect('mongodb://localhost/ng7crud?replicaSet=rs');
config = {
  DB: "mongodb://localhost:27017/test1", // database URL
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

const collection = db.collection("sampleCollection");

data = { key: "value", key2: 2 };

collection.insertOne(data, function (err, res) {
  if (err) throw err;
  console.log("1 document inserted");
  db.close();
});
