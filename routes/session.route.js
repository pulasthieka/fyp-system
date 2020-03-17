const express = require('express');
const sessionRoutes = express.Router();

// Require Business model in our routes module
let Session = require('../models/session');

// Defined store route
sessionRoutes.route('/add').post(function (req, res) {
  let business = new Session(req.body);
  business.save()
    .then(business => {
      res.status(200).json({'business': 'business in added successfully'});
    })
    .catch(err => {
    res.status(400).send("unable to save to database");
    });
});

// Defined get data(index or listing) route
sessionRoutes.route('/').get(function (req, res) {
    console.log('Backend Get')
    Session.find(function (err, businesses){
    if(err){
      console.log(err);
    }
    else {
      res.json(businesses);
    }
  });
});

// Defined edit route
sessionRoutes.route('/edit/:id').get(function (req, res) {
  let id = req.params.id;
  Session.findById(id, function (err, business){
      res.json(business);
  });
});

//  Defined update route
sessionRoutes.route('/update/:id').post(function (req, res) {
    Session.findById(req.params.id, function(err, next, business) {
    if (!business)
      return next(new Error('Could not load Document'));
    else {
        business.person_name = req.body.person_name;
        business.business_name = req.body.business_name;
        business.business_gst_number = req.body.business_gst_number;

        business.save().then(business => {
          res.json('Update complete');
      })
      .catch(err => {
            res.status(400).send("unable to update the database");
      });
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