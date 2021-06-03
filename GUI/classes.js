class Messenger {
  _type;
  _count;
  constructor(type) {
    this._type = type;
    this._count = 0;
  }
  send(message) {
    const type = this._type; // "PDATA";
    const data = message;
    let N = message.length;
    const time = Array.from({ length: N }, (_, index) => index + this._count);
    this._count += N;
    return { type, data, time };
    //   socket.emit(this._collectionName, {type,data})
  }
}

class Client {
  _collectionName;
  _changeStream;
  constructor(collectionName) {
    this._collectionName = collectionName;
  }

  createCollection() {
    createDocsIfNotExists(this._collectionName, bioSignals);
  }

  saveEvent(event) {
    console.log(`Saved ${event} : ${time} for ${this._collectionName}`);
  }
}

module.exports.Messenger = Messenger;
module.exports.Client = Client;
