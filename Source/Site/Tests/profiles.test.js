/******************************************************
* profiles.js - Travis Hodgin, Zach Lerew
* gets all profiles, current profile, create profile
******************************************************/
module.exports = function () {
    var express = require('express');
    var router = express.Router();
    const http = require("http");

    /* profile zones */
    let zones = require('../js/global/zones.js');
    let curr = require('../js/global/currentProfile.js');
    /* Connection settings */
    let conn = require('../js/global/connection.js');

    /* Control service request */
    let controlService = require('../js/global/controlServiceRequest.js');

    /* Daily State functions */
    let dailyStates = require('../js/global/dailyStates.js');

    /* LED functions */
    let leds = require('../js/global/leds.js');

    function getProfile(res, context, id, complete){
        let profileUrl = conn.url + conn.port + '/' + conn.profiles + '/' + id;
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
        var callbackCount = 0;
        var context = req.context;

        /* Get Daily states */
        dailyStates.get(res, context, complete);
        if(res.status == 200 ){
          console.log("SUCCESS: GET PROFILES STATUS: " + res.status);
        }
        }
    });


    router.post('/add', function (req, res) {
        console.log("POST ADD PROFILE");
        let j = {};
        j['name'] = req.body.name;
        j['description'] = req.body.description;

        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.profiles + "/add",
            json: j
        };
        controlService.makeRequest(options, function () {

          if(res.status == 200){
            console.log("SUCCESS: POST PROFILE STATUS: " res.status);
          }
          else{
            console.log("ERROR: POST PROFILE STATUS: " + res.status);
          }
      }
    });


    router.post('/edit', function (req, res) {
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
        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.post('/:id/zones/add', function (req, res) {
        let id = req.body.id;
        console.log("POST zone " + id);
        let j = {};
        j['name'] = req.body.name;

        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/zones/add",
            json: j
        };

        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.post('/:id/zones/leds/add', function (req, res) {
        let zoneId = req.body.id;

        console.log("POST zone leds for " + zoneId);
        let minID = parseInt(req.body.min);
        let maxID = parseInt(req.body.max);
        let range = maxID - minID;
        let j = [];
        for (let i = 0; i < range+1; i++) {
            j.push(minID + i);
        }

        var options = {
            method: "PUT",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/leds/add",
            json: j
        };

        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.post('/:id/zones/edit', function (req, res) {
        let zoneId = req.body.id;
        console.log("PUT zone daily states for zone " + zoneId);

        let days = [];

        let dailyState = parseInt(req.body.state);
        days[0] = (req.body.sunday == "on" ? dailyState : 0);
        days[1] = (req.body.monday == "on" ? dailyState : 0);
        days[2] = (req.body.tuesday == "on" ? dailyState : 0);
        days[3] = (req.body.wednesday == "on" ? dailyState : 0);
        days[4] = (req.body.thursday == "on" ? dailyState : 0);
        days[5] = (req.body.friday == "on" ? dailyState : 0);
        days[6] = (req.body.saturday == "on" ? dailyState : 0);

        var options = {
            method: "PUT",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/days/add/",
            json: days
        };

        controlService.makeRequest(options, function (err, response, body) {
            res.redirect('back');
        });

    });


    router.post('/:id/zones/delete', function (req, res) {
        let zoneId = req.body.id;
        console.log("DELETE zone " + zoneId);

        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/delete",
            json: {}
        };

        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.post('/:id/zones/leds/delete', function (req, res) {
        let zoneId = req.body.id;

        console.log("DELETE zone leds for " + zoneId);
        let minID = parseInt(req.body.min);
        let maxID = parseInt(req.body.max);
        let range = maxID - minID;
        let j = [];
        for (let i = 0; i < range+1; i++) {
            j.push(minID + i);
        }

        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + req.params.id + "/zones/" + zoneId + "/leds/delete",
            json: j
        };

        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.post('/delete', function (req, res) {
        let id = req.body.id;
        console.log("DELETE Profile " + id);
        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/delete"
        };
        controlService.makeRequest(options, function () {
            res.redirect('back');
        });
    });


    router.get('/:id', function (req, res) {
        callbackCount = 0;
        var context = {};
        getProfile(res, context, req.params.id, complete);
        curr.get(res, context, complete);
        zones.get(res, context, req.params.id, complete);
        leds.get(res, context, complete);
        dailyStates.get(res, context, complete)
        function complete() {
            callbackCount++;
            if (callbackCount >= 5) {
                res.render('update-zones', context);
            }
        }
    });

    return router;
}();
