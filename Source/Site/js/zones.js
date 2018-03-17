/******************************************************
* zones.js - Travis Hodgin,
* Gets everything needed to render the zones page
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  let conn = require('./global/connection.js');

  /* Current profile for navbar */
  let currentProfile = require('./global/currentProfile.js');

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

  router.get('/', function (req, res) {
    var callbackCount = 0;
    var context = {};

    /* Get all profiles */
    getAllProfiles(res, context, complete);

    /* Get current profile */
    currentProfile.get(res, context, complete);

    function complete() {
      console.log("in complete");
      callbackCount++;
      if (callbackCount >= 2) {
        console.log("ZONES!");
        res.render('zones', context);
      }
    }
  });


  return router;
}();
