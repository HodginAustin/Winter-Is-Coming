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

  return router;
}();
