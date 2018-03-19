/******************************************************
* Plant.js - Travis Hodgin,
* Gets everything needed to render the main plant page
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  let conn = require('./global/connection.js');
  
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
        res.render('plant', context);
      }
    }
  });


  return router;
}();
