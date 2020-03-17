const mongoose = require('mongoose');
const Schema = mongoose.Schema;

// Define collection and schema for a recording session
let Session = new Schema({
    Pressure: [{
        time_stamp: {
            type: String
          },
          value: {
            type: Number
          },
    }],
    Temperature:[{
        time_stamp: {
            type: String
          },
          value: {
            type: Number
          },
    }],
  },{
      collection: 'sessions'
  });
  
  module.exports = mongoose.model('Session', Session);