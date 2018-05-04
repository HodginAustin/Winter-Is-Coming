/******************************************************
* profiles.js - Travis Hodgin, Zach Lerew
* gets all profiles, current profile, create profile
******************************************************/
module.exports = function () {
    var express = require('express');
    var router = express.Router();
    const http = require("http");

    /* Profile Zones */
    let zones = require('./global/zones.js');
    let curr = require('./global/currentProfile.js');

    /* Connection settings */
    let conn = require('./global/connection.js');

    /* Control service request */
    let controlService = require('./global/controlServiceRequest.js');

    /* Daily State functions */
    let dailyStates = require('./global/dailyStates.js');

    /* LED functions */
    let leds = require('./global/leds.js');

    function getProfile(res, context, id, complete) {
        let profileUrl = conn.url + conn.port + '/' + conn.profiles + '/' + id;
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
        console.log("IN GET PROFILES");
        /* Get Daily states */
        dailyStates.get(res, context, complete);
        function complete() {
            callbackCount++;
            if (callbackCount >= 1) {
                res.render('profiles', context);
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
            res.redirect('back');
        });
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

        let days = {};

        let dailyState = parseInt(req.body.state);
        if (req.body.sunday == "on") { days["sunday"] = dailyState; }
        if (req.body.monday == "on") { days["monday"] = dailyState; }
        if (req.body.tuesday == "on") { days["tuesday"] = dailyState; }
        if (req.body.wednesday == "on") { days["wednesday"] = dailyState; }
        if (req.body.thursday == "on") { days["thursday"] = dailyState; }
        if (req.body.friday == "on") { days["friday"] = dailyState; }
        if (req.body.saturday == "on") { days["saturday"] = dailyState; }

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
        var context =req.context;

        getProfile(res, context, req.params.id, complete);
        curr.get(res, context, complete);
        zones.get(res, context, req.params.id, complete);
        leds.get(res, context, complete);
        dailyStates.get(res, context, complete);

        function complete() {
            callbackCount++;
            if (callbackCount >= 5) {
                res.render('update-zones', context);
            }
        }
    });

    return router;
}();
