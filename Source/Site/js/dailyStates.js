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

  /* Current profile for navbar */
  let currentProfile = require('./global/currentProfile.js');
  
  router.get('/', function (req, res) {
    var callbackCount = 0;
    var context = {};

    /* Get current profile */
    currentProfile.get(res, context, complete);

    function complete() {
      callbackCount++;
      if (callbackCount >= 1) {
        console.log("Daily States!");
        res.render('dailyStates', context);
      }
    }
  });


  return router;
}();
