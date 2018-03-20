/******************************************************
* profiles.js - Travis Hodgin, Zach Lerew
* gets all profiles, current profile, create profile
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* profile zones */
  let zones = require('./global/zones.js');
  let curr = require('./global/currentProfile.js');
  /* Connection settings */
  let conn = require('./global/connection.js');

  /* Control service request */
  let controlService = require('./global/controlServiceRequest.js');

  let dailyStates = require('./global/dailyStates.js');

  function getProfile(res, context, id, complete){
    var profileUrl = conn.url + conn.port +'/' + conn.profiles + '/' + id;
    console.log("getProfile url: " + profileUrl);
    http.get(profileUrl, res => {
      res.setEncoding("utf8");
      body = "";
      res.on("data", data => {
        body += data;
      });
      res.on("end", () => {
        body = JSON.parse(body);
        context.Profile = body;
        complete();
      });
    });
  }
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

  router.get('/:id', function(req, res){
    callbackCount = 0;
    var context = {};
    getProfile(res, context, req.params.id, complete);
    curr.get(res, context, complete);
    zones.get(res, context, req.params.id, complete);
    dailyStates.get(res, context, complete)
    function complete(){
      callbackCount++;
      if(callbackCount >= 4){
        res.render('update-zones', context);
      }
    }
  });


  return router;
}();
