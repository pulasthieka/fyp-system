var mongoose = require('mongoose');
config = {
    DB: 'mongodb://localhost:27017/tryone?replicaSet=rs',
};
mongoose.connect(config.DB, { useNewUrlParser: true, useUnifiedTopology: true }).then(
    () => {
        console.log('Database is connected');
    },
    (err) => {
        console.log('Can not connect to the database' + err);
    }
);
var recording = {
    pat: { time: [], value: [] },
    temp: { time: [], value: [] },
    imp: { time: [], value: [] },
};
console.log(recording);
const db = mongoose.connection;

db.on('error', console.error.bind(console, 'Connection Error:'));
let collectionName = 'a';
var document = db.collection(collectionName);
addDatatoDocument(document, recording);
// document.insertOne(recording);
// document.updateOne({ name: 'task.docname' }, { $push: { ['P.1']: { $each: 'task.processed_data' } } }, (err) => {
//     if (err) console.log('DB error:', err);
// });
// console.log(document);
async function addDatatoDocument(collection, data) {
    await collection.insertOne(data);
}
async function update(collection, data){
    collection.updateOne({ name: data.document }, { $push: { [field]: { $each: 'task.processed_data' } } }, (err) => {
    if (err) console.log('DB error:', err);
});
}