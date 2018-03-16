/******************************************************
* ledStates.js - Travis Hodgin,
* Gets everything needed to render the ledState page
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  var conn = require('./global/connection.js');

  /*gets all profiles. Uses async to collect data and complete to render */
  function getAllProfiles(res, context, complete) {
    var profileURL = conn.url + conn.port + '/' + conn.profiles;
    http.get(profileURL, res => {
      res.setEncoding("utf8");
      body = "";
      res.on("data", data => {
        body += data;
      });
      res.on("end", () => {
        body = JSON.parse(body);
        context.Profiles = body;
        complete();
      });
    });
  }

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

  /*gets current profile. Uses async to collect data and complete to render
    THIS FUNCTION SHOULD BE INCLUDED IN ALL PAGES FOR THE NAVBAR CURRENT PROFILE*/
  function getCurrentProfile(res, context, complete) {
    var current = conn.url + conn.port + '/' + conn.currentProfile;
    http.get(current, res => {
      res.setEncoding("utf8");
      body = "";
      res.on("data", data => {
        body += data;
      });
      res.on("end", () => {
        body = JSON.parse(body);
        context.currentProfile = body;
        complete();
      });
    });
  }

  router.get('/', function (req, res) {
    var callbackCount = 0;
    var context = {};
    getAllLEDStates(res, context, complete);
    getAllProfiles(res, context, complete);
    getCurrentProfile(res, context, complete);
    function complete() {
      callbackCount++;
      if (callbackCount >= 3) {
        res.render('ledStates', context);
      }
    }
  });



  return router;

}();
