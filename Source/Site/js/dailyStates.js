/******************************************************
* dailyStates.js - Travis Hodgin, Zach Lerew
* Gets everything needed to render the dailyState plant page
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

    /* LED State functions */
    let ledStates = require('./global/ledStates.js');

    /* Daily State functions */
    let dailyStates = require('./global/dailyStates.js');

    /* Converts hour, minute, second into seconds after midnight */
    function get_time(req) {
        let time = 0;
        let hour = parseInt(req.body.hour) || 0;
        let minute = parseInt(req.body.minute) || 0;
        let second = parseInt(req.body.second) || 0;
        let ampm = req.body.ampm;

        hour *= (ampm == "pm" ? 12 : 1);
        return (hour * 60 * 60) + (minute * 60) + (second);
    }

    /* Get page */
    router.get('/', function (req, res) {
        var callbackCount = 0;
        var context = {};

        /* Get Daily states */
        dailyStates.get(res, context, complete);

        /* Get current profile */
        currentProfile.get(res, context, complete);

        /* LED States */
        ledStates.get(res, context, complete);

        function complete() {
            callbackCount++;
            if (callbackCount >= 3) {
                res.render('dailyStates', context);
            }
        }
    });


    /* Add */
    router.post('/addMapping', function (req, res) {
        let dailyStateID = req.body.id;

        console.log("POST dailyState mapping for " + id);

        // Get form data
        let time = get_time(req);
        let state = parseInt(req.body.state);

        // Build JSON for control service
        let j = {};
        j['time'] = time;
        j['state'] = state;
        j = [j];

        // Patch control service
        var options = {
            method: "PUT",
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/" + dailyStateID + "/led_states/add",
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
        console.log("PATCH dailyState " + id);

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
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/" + id + "/edit",
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
    router.post('/deleteMapping', function (req, res) {
        let id = req.body.id;
        let time = req.body.time;
        console.log("DELETE dailyState mapping for " + id);

        // Patch control service
        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/" + id + "/led_states/" + time + "/delete"
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
