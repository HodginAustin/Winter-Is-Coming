/******************************************************
 * settings.js - Travis Hodgin, Zach Lerew
 * Gets everything needed to render the settings page
 ******************************************************/

module.exports = function() {
    var express = require('express');
    var router = express.Router();

    /* Connection settings */
    var conn = require('./global/connection.js');

    /* Control service request */
    let controlService = require('./global/controlServiceRequest.js');

    /* Get page */
    router.get('/', function(req, res) {
        var context = req.context;

        res.render('settings', context);
    });


    /* Delete */
    router.post('/erase_all', function(req, res) {
        console.log("DELETE NUKE FROM ORBIT (It's the only option)");

        let confirm1 = req.body.confirm1;
        let confirm2 = req.body.confirm2;
        let confirm3 = req.body.confirm3;

        if (confirm1 != "delete" || confirm2 != "all" || confirm3 != "data") {
            let ErrorPage = "";
            ErrorPage += "<h3>Incorrect confirmation words, nothing was deleted. Try again!</h3>";
            ErrorPage += "<br/>";
            ErrorPage += "<a href='/plant'>Go Home</a>";
            res.send(ErrorPage);
        } else {
            // control service options
            var options = {
                method: "DELETE",
                uri: conn.url + conn.port + "/nuke_from_orbit",
                json: {}
            };

            // Make request
            controlService.makeRequest(options, function() {
                res.render('delete_success', {});
            })
        }
    });

    /* Program shutdown */
    router.post('/shutdown', function(req, res) {
        console.log("GET SERVICE SHUTDOWN");

        // control service options
        var options = {
            method: "GET",
            uri: conn.url + conn.port + "/shutdown",
            json: {}
        };

        // Make request
        controlService.makeRequest(options, function() {
            res.redirect('../');
        })
    });

    /* System power off */
    router.post('/system_shutdown', function(req, res) {
        console.log("GET SYSTEM SHUTDOWN");

        // control service options
        var options = {
            method: "GET",
            uri: conn.url + conn.port + "/system_shutdown",
            json: {}
        };

        // Make request
        controlService.makeRequest(options, function() {
            res.redirect('../');
        })
    });

    /* System reboot */
    router.post('/system_reboot', function(req, res) {
        console.log("GET SYSTEM REBOOT");

        // control service options
        var options = {
            method: "GET",
            uri: conn.url + conn.port + "/system_reboot",
            json: {}
        };

        // Make request
        controlService.makeRequest(options, function() {
            res.redirect('../');
        })
    });

    return router;
}();