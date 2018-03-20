/******************************************************
* profiles.js - Travis Hodgin,
* gets all profiles, current profile, create profile
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  let conn = require('./global/connection.js');

  /* Control service request */
  let controlService = require('./global/controlServiceRequest.js');

  router.get('/', function (req, res) {
    var context = req.context;

    res.render('profiles', context);
  });

  router.post('/add', function(req, res){
        console.log("POST ADD PROFILE");
        let j = {};
        j['name'] = req.body.name;
        j['description'] = req.body.description;
        j['zones'] = req.body.zones;

        var options = {
          method: "POST",
          uri: conn.url + conn.port + "/" + conn.profiles + "/add",
          json: j
        };
        controlService.makeRequest(options, function(){
          res.redirect('back');
        });
     });


  router.post('/edit', function(req, res){
    let id = req.body.id;
    console.log("PATCH profile " + id);
    let j = {};
    j['name'] = req.body.name;
    j['description'] = req.body.description;
    j['zones'] = req.body.zones;
    var options = {
      method: "PATCH",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/edit",
      json: j
    };
    controlService.makeRequest(options, function(){
      res.redirect('back');
    });
  });

  router.post('/delete', function(req, res){
    let id = req.body.id;
    console.log("DELETE Profile " + id);
    var options = {
      method: "DELETE",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/delete"
    };
    controlService.makeRequest(options, function(){
      res.redirect('back');
    });
  });


  return router;
}();
