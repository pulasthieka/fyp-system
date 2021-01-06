# FYP Web Application

## Prerequisites

The following packages have to be installed.

1. [MongoDB](https://docs.mongodb.com/manual/installation/),
2. NodeJS,

## First Time install

#### Setup Database

1. Create a new database
2. Setup replica set for the new database
   - Start mongodb `mongodb --dbpath "path-to-database"`
   - Open a new terminal and start a mongo shell `monogo`
   - Type `rs.initiate()`

#### Setup Web-app

1. Install all packages with `npm install`
2. Point the server to database
   - Change config parameter `config = { DB: "mongodb://localhost:27017/dbname?replicaSet=rs"};`

## Startup Procedure

Run the below commands in seperate terminals

1. Start database with replicaset
   `mongod --dbpath "path-to-database" --replSet "rs"`

2. Start node server `nodemon server.js` or `node server.js`

3. Open `index.html` in a browser

## Navigating the graphs

- Click and drag to zoom
- Click and drag to create a negative area to switch to horizontal / vertical zoom
- Mousewheel doesn't work

You can also use the buttons above the graph to navigate the graph.

## Further help

Check [Hardware](/Hardware) for instructions on setting up the hardware
