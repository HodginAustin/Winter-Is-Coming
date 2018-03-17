/******************************************************
* ledStates.js - Travis Hodgin,
* Gets everything needed to render the ledState page
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");
  var request = require('request');

  /* Connection settings */
  var conn = require('./global/connection.js');

  /* Current profile for navbar */
  let currentProfile = require('./global/currentProfile.js');

  /* Get LED states */
  function getAllLEDStates(res, context, complete) {
    var ledStatesUrl = conn.url + conn.port + '/' + conn.ledStates;
    http.get(ledStatesUrl, res => {
      res.setEncoding("utf8");
      body = "";
      res.on("data", data => {
        body += data;
      });
      res.on("end", () => {
        body = JSON.parse(body);
        context.LEDStates = body;
        complete();
      });
    });
  }


  /* Get page */
  router.get('/', function (req, res) {
    var callbackCount = 0;
    var context = {};

    /* Get LED states */
    getAllLEDStates(res, context, complete);

    /* Get current profile */
    currentProfile.get(res, context, complete);

    function complete() {
      callbackCount++;
      if (callbackCount >= 2) {
        res.render('ledStates', context);
      }
    }
  });


  /* Add */
  router.post('/add', function (req, res) {
    console.log("POST ledstate");

    // Build JSON for control service
    let j = {};
    j['r'] = parseInt(req.body.red);
    j['g'] = parseInt(req.body.green);
    j['b'] = parseInt(req.body.blue);
    j['intensity'] = parseInt(req.body.intensity);
    j['power'] = req.body.power == "true";

    // Patch control service
    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.ledStates + "/add",
      json: j
    };

    // Make request
    request(options, function (err, response, body) {
      if (err || response.statusCode != 200) {
        let error = "Error! Status: " + res.statusCode + ", Response: " + String(response.body);
        console.log(error);
        console.log(response.body);
      }
      else {
        console.log("Control Service Response:" + response.body);
        res.redirect('back');
      }
    });
  });


  /* Update */
  router.post('/edit', function (req, res) {
    let id = req.body.id;
    console.log("PATCH ledstate " + id);

    // Build JSON for control service
    let j = {};
    j['r'] = parseInt(req.body.red);
    j['g'] = parseInt(req.body.green);
    j['b'] = parseInt(req.body.blue);
    j['intensity'] = parseInt(req.body.intensity);
    j['power'] = req.body.power == "true";

    // Patch control service
    var options = {
      method: "PATCH",
      uri: conn.url + conn.port + "/" + conn.ledStates + "/" + id + "/edit",
      json: j
    };

    // Make request
    request(options, function (err, response, body) {
      if (err || response.statusCode != 200) {
        let error = "Error! Status: " + res.statusCode + ", Response: " + String(response.body);
        console.log(error);
        console.log(response.body);
      }
      else {
        console.log("Control Service Response:" + response.body);
        res.redirect('back');
      }
    });
  });


  /* Delete */
  router.post('/delete', function (req, res) {
    let id = req.body.id;
    console.log("DELETE ledstate " + id);

    // Patch control service
    var options = {
      method: "DELETE",
      uri: conn.url + conn.port + "/" + conn.ledStates + "/" + id + "/delete"
    };

    // Make request
    request(options, function (err, response, body) {
      if (err || response.statusCode != 200) {
        let error = "Error! Status: " + res.statusCode + ", Response: " + String(response.body);
        console.log(error);
        console.log(response.body);
      }
      else {
        console.log("Control Service Response:" + response.body);
        res.redirect('back');
      }
    });
  });


  return router;
}();
