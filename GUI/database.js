const queue = require('async/queue'); // use queues to maintain application speed
const mongoose = require('mongoose'); // connect to MongoDB using mongoose

function connect(database) {
  const dbConfig = {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    useCreateIndex: true,
    useFindAndModify: false,
    autoIndex: false,
  };

  try {
    const connection = mongoose.connect(this._url + database, dbConfig);
    return connection;
  } catch (err) {
    console.error('Cannot connect to Database', err);
  }
  return false;
}

function createSignalModel(collectionName) {
  delete mongoose.connection.models[collectionName + 'SignalSchema'];
  const signalSchema = new mongoose.Schema(
    {
      time: [[Number]],
      data: [[Number]],
      name: { type: String, required: true },
    },
    { collection: collectionName }
  );
  return mongoose.model(collectionName + 'SignalSchema', signalSchema);
}

function createUserModel(collectionName) {
  delete mongoose.connection.models[collectionName + 'UserSchema'];
  const userSchema = new mongoose.Schema(
    {
      fname: { type: String, required: true },
      lname: { type: String, required: true },
      age: { type: Number, required: true },
      height: Number,
      weight: Number,
      gender: { type: String, required: false },
      phone: Number,
      condition: { type: String, required: false },
      comment: { type: String, required: false },
    },
    { collectionName: collectionName }
  );
  return mongoose.model(collectionName + 'UserSchema', userSchema);
}

function createEventModel(collectionName) {
  delete mongoose.connection.models[collectionName + 'EventSchema'];
  const eventSchema = new mongoose.Schema(
    {
      name: { type: String, required: true },
      baseline: [[String]],
      occlusion: [[String]],
      release: [[String]],
    },
    { collectionName: collectionName }
  );
  console.log(eventSchema);
  return mongoose.model(collectionName + 'EventSchema', eventSchema);
}

async function create(model, doc) {
  try {
    const res = await model.create(doc);
    console.log(`Created document for ${doc}`, res);
  } catch (error) {
    console.error(`Cannot creat document for ${doc}`, error);
  }
}

async function update(model, name, operation) {
  try {
    const res = await model.updateOne({ name: name }, operation, { upsert: true });
    console.log(`Updated document for ${name}`, res);
  } catch (error) {
    console.error(`Cannot update document for ${name}`, error);
  }
}

async function remove(model, name) {
  try {
    const res = await model.deleteOne({ name: name });
    console.log(`Removed document for ${name}`, res);
  } catch (error) {
    console.error(`Cannot remove document for ${name}`, error);
  }
}
class Database {
  _collectionName;
  _userModel;
  _signalModel;
  _eventModel;
  _documents;
  _connection;

  constructor(collectionName, documents) {
    this._collectionName = collectionName;
    this._userModel = createUserModel(collectionName);
    this._signalModel = createSignalModel(collectionName);
    this._eventModel = createEventModel(collectionName);
    this._documents = documents;
  }

  async connect(url) {
    const dbConfig = {
      useNewUrlParser: true,
      useUnifiedTopology: true,
      useCreateIndex: true,
      useFindAndModify: false,
      autoIndex: false,
    };

    try {
      await mongoose.connect(url, dbConfig);
      return true;
    } catch (err) {
      console.error('Cannot connect to Database', err);
    }
    return false;
  }

  async saveUser(record) {
    create(this._userModel, record); //save user
  }

  async initiate() {
    create(this._eventModel, { name: 'event', baseline: [], occlusion: [], release: [] });
    this._documents.forEach((doc) => {
      const record = {
        name: doc,
        time: [],
        data: [],
      };
      create(this._signalModel, record);
    });
  }

  // create for each biosignal
  updateSignals(signal, data, time) {
    const query = { $push: { ['data']: { $each: data }, ['time']: { $each: time } } };
    update(this._signalModel, signal, query);
  }

  updateEvents(event, data) {
    const query = { $push: { [event]: { $each: data } } }; // implement check for event type
    update(this._eventModel, 'event', query);
  }

  remove() {
    remove(this._eventModel, 'event');
    this._documents.forEach((doc) => {
      remove(this._signalModel, doc);
    });
  }
}

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

async function clearPlots() {
  createDocsIfNotExists(collectionName, prefixes1);
  console.log('Collection: ', collectionName, ' reset');
}

module.exports.Database = Database;
