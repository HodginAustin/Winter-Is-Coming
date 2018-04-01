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

    var options = {
      method: "PATCH",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/edit",
      json: j
    };
    controlService.makeRequest(options, function(){
      res.redirect('back');
    });
  });

  router.post('/:id/zones/add', function(req, res){
    let id = req.body.id;
    console.log("POST zone " + id);
    let j = {};
    j['name'] = req.body.name;

    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/zones/add",
      json: j
    };
    
    controlService.makeRequest(options, function(){
      res.redirect('back');
    });
  });

  router.post('/:id/zones/leds/add', function(req, res){
    let zoneId = req.body.id;

    console.log("POST zone leds for " + zoneId);
    let j = req.body.leds.split(',');
    j = j.map(id => parseInt(id));


    var options = {
      method: "PUT",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/leds/add",
      json: j
    };
    
    controlService.makeRequest(options, function(){
      res.redirect('back');
    });
  });
  
  router.post('/:id/zones/edit', function(req, res){
    let zoneId = req.body.id;
    console.log("PUT zone daily states for " + zoneId);
    
    let sunday = req.body.sunday;
    let monday = req.body.monday;
    let tuesday = req.body.tuesday;
    let wednesday = req.body.wednesday;
    let thursday = req.body.thursday;
    let friday = req.body.friday;
    let saturday = req.body.saturday;
    
    let allReqs = [];
    let successCount = 0;

    if (sunday != undefined) {
      var dow = 0;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + sunday,
        json: {}
      };
      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      }
    }
    if (monday != undefined) {
      var dow = 1;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + monday,
        json: {}
      };
      
      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      }
    }
    if (tuesday != undefined) {
      var dow = 2;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + tuesday,
        json: {}
      };

      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      }
    }
    if (wednesday != undefined) {
      var dow = 3;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + wednesday,
        json: {}
      };

      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      }
    }
    if (thursday != undefined) {
      var dow = 4;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + thursday,
        json: {}
      };
      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      } 
    }
    if (friday != undefined) {
      var dow = 5;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + friday,
        json: {}
      };
      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
      }
    }
    if (saturday != undefined) {
      var dow = 6;
      var options = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/day/" + dow + "/add/" + saturday,
        json: {}
      };
      while (successCount <= dow) {
        controlService.makeRequest(options, function(){
          reqDone();
        });
        break;
        
      }
    }


    function reqDone() {
      successCount++;
      if (successCount >= 7) {
        res.redirect('back');
      }
    }

  });


  router.post('/:id/zones/delete', function(req, res){
    let zoneId = req.body.id;
    console.log("DELETE zone " + zoneId);

    var options = {
      method: "DELETE",
      uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/delete",
      json: {}
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
