# FYP Web Application

## Prerequisites

The following packages have to be installed.

1. [NodeJS](https://nodejs.org/en/)
2. [MongoDB](https://docs.mongodb.com/manual/installation/)

## First Time install

### Setup Databases

- Create a new database. eg: `fyp`
- _OPTIONAL_ - Setup replica set for the new database
  - Navigate to the MongoDB installation path
  - Open a new terminal and start a mongo shell `mongo`
  - Type `rs.initiate()` (Make sure other there are no other running mongoDB services)

### Setup Webserver

1. Install all packages with `npm install`
2. If any changes were made to the hardware/ database setup, change the `config` variable in the [server.js](./server.js) file. Point the server to database and specify the ports for the gui and hardware.

```js
const config = {
  DB: 'mongodb://127.0.0.1:27017/', // database URL
  PORT: 9090, // GUI port see it at localhost:9090
  WS_PORT: 8011, // hardware connection port
};
```

## Startup Procedure

Run the below commands in seperate terminals

1. Start database. Application can also work without a database. However, this means collected data will not be saved.

   ```bash
   mongod --dbpath "path-to-database"
   ```

2. Start node server.

   ```bash
   node server.js
   #or nodemon server.js
   ```

3. Go to [http://localhost:90/plots?id=defaultCollection](http://localhost:90/plots?id=defaultCollection) to directly view the plots page.

Once hardware is connected, then data collected from the hardware will the plotted on the `plots_page.html`

## Navigating the graphs

- Click and drag to zoom
- Click and drag to create a negative area to switch to horizontal / vertical zoom
- Mousewheel doesn't work

You can also use the buttons above the graph to navigate the graph.

## Further help

Check [Hardware](../Hardware) for instructions on setting up the hardware
