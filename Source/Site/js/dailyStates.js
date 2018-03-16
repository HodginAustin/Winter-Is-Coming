/******************************************************
* dailyStates.js - Travis Hodgin,
* Gets everything needed to render the dailyState plant page
******************************************************/
var dailyStates = "daily_states";
var profiles = "profiles"; /* grab profile */
var currentProfile = "current_profile";
var ledStates = "led_states";

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  var conn = require('./global/connection.js');

  /*gets all profiles. Uses async to collect data and complete to render */
  function getAllProfiles(res, context, complete) {
    var profileURL = conn.url + conn.port + '/' + profiles;
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

  /*gets current profile. Uses async to collect data and complete to render
    THIS FUNCTION SHOULD BE INCLUDED IN ALL PAGES FOR THE NAVBAR CURRENT PROFILE*/
  function getCurrentProfile(res, context, complete) {
    var current = conn.url + conn.port + '/' + currentProfile;
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
    getAllProfiles(res, context, complete);
    getCurrentProfile(res, context, complete);
    function complete() {
      console.log("in complete");
      callbackCount++;
      if (callbackCount >= 2) {
        console.log("ZONES!");
        res.render('dailyStates', context);
      }
    }
  });



  return router;

}();
