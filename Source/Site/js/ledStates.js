/******************************************************
* ledStates.js - Travis Hodgin, Zach Lerew
* Gets everything needed to render the ledState page
******************************************************/

module.exports = function () {
    var express = require('express');
    var router = express.Router();

    /* Connection settings */
    var conn = require('./global/connection.js');

    /* LED State functions */
    let ledStates = require('./global/ledStates.js');

    /* Control service request */
    let controlService = require('./global/controlServiceRequest.js');

    /* Get page */
    router.get('/', function (req, res) {
        var callbackCount = 0;
        var context = req.context;

        /* Get LED states */
        ledStates.get(res, context, complete);

        function complete() {
            callbackCount++;
            if (callbackCount >= 1) {
                res.render('ledStates', context);
            }
        }
    });


    /* Add */
    router.post('/add', function (req, res) {
        console.log("POST ledstate");

        // Build JSON for control service
        let j = {};
        j['r'] = parseInt(req.body.red);
        j['g'] = parseInt(req.body.green);
        j['b'] = parseInt(req.body.blue);
        j['intensity'] = parseInt(req.body.intensity);
        j['power'] = req.body.power == "true";

        // Patch control service
        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.ledStates + "/add",
            json: j
        };

        // Make request
        controlService.makeRequest(options, function () { res.redirect('back'); })
    });


    /* Update */
    router.post('/edit', function (req, res) {
        let id = req.body.id;
        console.log("PATCH ledstate " + id);

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
            uri: conn.url + conn.port + "/" + conn.ledStates + "/" + id + "/edit",
            json: j
        };

        // Make request
        controlService.makeRequest(options, function () { res.redirect('back'); })
    });


    /* Delete */
    router.post('/delete', function (req, res) {
        let id = req.body.id;
        console.log("DELETE ledstate " + id);

        // Patch control service
        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.ledStates + "/" + id + "/delete"
        };

        // Make request
        controlService.makeRequest(options, function () { res.redirect('back'); })
    });


    return router;
}();
