/******************************************************
* dailyStates.js - Travis Hodgin, Zach Lerew
* Gets everything needed to render the dailyState plant page
******************************************************/

module.exports = function () {
    var express = require('express');
    var router = express.Router();

    /* Connection settings */
    var conn = require('./global/connection.js');

    /* LED State functions */
    let ledStates = require('./global/ledStates.js');

    /* Daily State functions */
    let dailyStates = require('./global/dailyStates.js');

    /* Control service request */
    let controlService = require('./global/controlServiceRequest.js');

    /* Converts hour, minute, second into seconds after midnight */
    function get_time(req) {
        let time = 0;
        let hour = parseInt(req.body.hour) || 0;
        let minute = parseInt(req.body.minute) || 0;
        let second = parseInt(req.body.second) || 0;
        let ampm = req.body.ampm;

        if (ampm == "pm") {
            hour += 12;
        } else if (ampm == "am" && hour==12) {
            hour = 0;
        }
        let t = (hour * 60 * 60) + (minute * 60) + (second);

        return t;
    }

    /* Get page */
    router.get('/', function (req, res) {
        var callbackCount = 0;
        var context = req.context;

        /* Get Daily states */
        dailyStates.get(res, context, complete);

        /* LED States */
        ledStates.get(res, context, complete);

        function complete() {
            callbackCount++;
            if (callbackCount >= 2) {
                res.render('dailyStates', context);
            }
        }
    });


    /* Add */
    router.post('/addMapping', function (req, res) {
        let dailyStateID = req.body.id;

        console.log("POST dailyState mapping for " + dailyStateID);

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
        controlService.makeRequest(options, function () { res.redirect('back'); })
    });
    router.post('/add', function (req, res) {
        console.log("POST dailyState");

        // Patch control service
        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/add",
            json: {}
        };

        // Make request
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
    });


    /* Update */
    router.post('/edit', function (req, res) {
        let id = req.body.id;
        console.log("PUT dailyState " + id);

        // Build JSON for control service
        let j = {};
        j['r'] = parseInt(req.body.red);
        j['g'] = parseInt(req.body.green);
        j['b'] = parseInt(req.body.blue);
        j['intensity'] = parseInt(req.body.intensity);
        j['power'] = req.body.power == "true";

        // Patch control service
        var options = {
            method: "PUT",
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/" + id + "/edit",
            json: j
        };

        // Make request
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
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
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
    });

    router.post('/delete', function (req, res) {
        let id = req.body.id;
        console.log("DELETE dailyState " + id);

        // Patch control service
        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.dailyStates + "/" + id + "/delete/"
        };

        // Make request
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
    });

    return router;
}();
