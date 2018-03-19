/******************************************************
* hardware.js - Travis Hodgin, Zach Lerew
* Gets everything needed to render the hardware plant page
******************************************************/

module.exports = function () {
    var express = require('express');
    var router = express.Router();

    /* Connection settings */
    var conn = require('./global/connection.js');

    /* Current profile for navbar */
    let currentProfile = require('./global/currentProfile.js');

    /* Controller functions */
    let controllers = require('./global/controllers.js');

    /* LED functions */
    let leds = require('./global/leds.js');

    /* Control service request */
    let controlService = require('./global/controlServiceRequest.js');

    /* Get page */
    router.get('/', function (req, res) {
        let callbackCount = 0;
        let context = {};
        let controllersList = {};
        let ledsList = {};

        /* Get LED controllers */
        controllers.get(res, controllersList, SortLEDs);

        /* Loop through LEDs and dump them into the appropriate controller */
        function SortLEDs() {
            /* Get LEDs */
            leds.get(res, ledsList, gotLEDs);

            function gotLEDs() {
                // Add empty LEDs list to each controller
                for (let c = 0; c < controllersList.Controllers.length; c++) {
                    controllersList.Controllers[c].LEDs = [];
                    controllersList.Controllers[c].LEDNextIdx = 0;
                }

                // Assign LEDs
                for (let l = 0; l < ledsList.LEDs.length; l++) {
                    let led = ledsList.LEDs[l];

                    for (let c = 0; c < controllersList.Controllers.length; c++) {
                        let controller = controllersList.Controllers[c];

                        if (led.controller == controller.id) {
                            controller.LEDs.push(led);
                            controller.LEDNextIdx = Math.max(controller.LEDNextIdx, led.strip_idx + 1);
                        }
                    }
                }

                context = controllersList;

                complete();
            }
        }

        /* Get current profile */
        currentProfile.get(res, context, complete);

        function complete() {
            callbackCount++;
            if (callbackCount >= 2) {
                res.render('hardware', context);
            }
        }
    });


    /* Add */
    router.post('/led/add', function (req, res) {
        let controllerId = req.body.id;

        console.log("POST LED for controller " + controllerId);

        // Get form data
        let nextIdx = req.body.idx;
        let count = req.body.count;

        let j = [];
        for (let i = 0; i < count; i++) {
            // Build JSON for control service
            let t = {};
            t['controller'] = parseInt(controllerId);
            t['strip_idx'] = parseInt(nextIdx) + i;
            j.push(t);
        }
        // Control service options
        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.leds + "/add",
            json: j
        };

        // Make request
        controlService.makeRequest(options, function () { res.redirect('back'); })
    });
    router.post('/add', function (req, res) {
        console.log("POST controller");

        // Build JSON for control service
        let j = {};
        j['io'] = parseInt(req.body.io);

        // Control service options
        var options = {
            method: "POST",
            uri: conn.url + conn.port + "/" + conn.controllers + "/add",
            json: j
        };

        // Make request
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
    });


    /* Delete */
    router.post('/leds/delete', function (req, res) {
        // Get form data
        let controllerId = req.body.controllerId;
        let count = req.body.count;
        console.log("DELETE " + count + "LEDs for controller " + controllerId);

        for (let i = 0; i < count; i++) {
            // Control service options
            var options = {
                method: "DELETE",
                uri: conn.url + conn.port + "/" + conn.leds + "/" + ledId + "/delete/"
            };

            // Make request
            controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
        }
    });

    router.post('/delete', function (req, res) {
        let id = req.body.id;
        console.log("DELETE controller " + id);

        // Control service options
        var options = {
            method: "DELETE",
            uri: conn.url + conn.port + "/" + conn.controllers + "/" + id + "/delete/"
        };

        // Make request
        controlService.makeRequest(options, function (err, response, body) { res.redirect('back'); })
    });

    return router;
}();
