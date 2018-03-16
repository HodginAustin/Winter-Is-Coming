/******************************************************
* profiles.js - Travis Hodgin,
* gets all profiles, current profile, create profile
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

  /*gets all profiles. Uses async to collect data and complete to render NEEDED ON ALL VIEWS */
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

  function getProfile(res, context, id, complete) {
    var profileURL = conn.url + conn.port + '/' + conn.profiles + "/" + id;
    http.get(profileURL, res => {
      res.setEncoding("utf8");
      body = "";
      res.on("data", data => {
        console.log(data);
        body += data;
      });
      res.on("end", () => {
        body = JSON.parse(body);
        context.Profile = body
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
        console.log("");
        res.render('profiles', context);
      }
    }
  });

  router.get('/:id', function (req, res) {
    var callbackCount = 0;
    var context = {};
    console.log("id: " + req.params.id);
    getProfile(res, context, req.params.id, complete);
    function complete() {
      callbackCount++;
      if (callbackCount >= 1) {
        res.render('update-profile', context);
      }
    }
  });


  return router;
}();
