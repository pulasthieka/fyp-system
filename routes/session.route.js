const express = require('express');
const sessionRoutes = express.Router();

// Require Business model in our routes module
let Session = require('../models/session');
var id = 0;

sessionRoutes.route('/new').post(function (req, res) {
  let business = new Session(req.body);
  business.save()
    .then(business => {
      console.log('New session added ' + business._id);
      res.status(200).json({'id': business._id});
    })
    .catch(err => {
    res.status(400).send("unable to save to database");
    });
});

// Defined store route
sessionRoutes.route('/add/:id').post(function (req, res) {
    id = req.params.id
    Session.findById(req.params.id, function(err, business) {
        if (!business){
          // following code creates a new session if a session is not found
            let business = new Session(req.body);
            business.save()
              .then(business => {
                console.log('New business added ' + business._id);
                res.status(200).json({'id': business._id});
              })
              .catch(err => {
              res.status(400).send("unable to save to database");
              });
            // return next(new Error('Could not load Document'));
        }
        
        else {
            let p = req.body.Pressure;
            let t = req.body.Temperature;
            // business.Pressure.value = req.body.Pressure.value;
            // business.business_gst_number = req.body.business_gst_number;
            console.log(p,t)
           Session.updateOne({"_id":id},{$push:{Pressure:p,Temperature:t}}).then(business => {
              res.json({'id':id,'status':'Update complete'});
          })
          .catch(err => {
                res.status(400).send("unable to update the database");
          });
        }
      });
});

// Defined get data(index or listing) route
sessionRoutes.route('/get/:id').get(function (req, res) {
    console.log('Backend Get')
    Session.findById(req.params.id, function (err, businesses){
    if(err){
      console.log(err);
    }
    else {
      res.json(businesses);
    }
  });
});


// Defined delete | remove | destroy route
sessionRoutes.route('/delete/:id').get(function (req, res) {
    Session.findByIdAndRemove({_id: req.params.id}, function(err, business){
        if(err) res.json(err);
        else res.json('Successfully removed');
    });
});

module.exports = sessionRoutes;