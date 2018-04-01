/*gets current profile. Uses async to collect data and complete to render
THIS FUNCTION SHOULD BE INCLUDED IN ALL PAGES FOR THE NAVBAR CURRENT PROFILE*/
function get(res, context, complete) {
    let conn = require('./connection.js');
    const http = require("http");

    /* Control service request */
    let controlService = require('./controlServiceRequest.js');

    // Control service options
    let options = {
        method: "GET",
        uri: conn.url + conn.port + '/' + conn.currentProfile,
        json: {}
    };

    // Make request
    controlService.makeRequest(options,
        function (err, response, body) {
            context.currentProfile = body;
            complete();
        })
}

function set(res, profileId, complete) {
    var conn = require('./connection.js');
    const http = require("http");

    /* Control service request */
    let controlService = require('./controlServiceRequest.js');

    // Patch control service
    var options = {
        method: "POST",
        uri: conn.url + conn.port + '/' + conn.currentProfile + '/' + profileId,
        json: {}
    };

    // Make request
    controlService.makeRequest(options, function () { complete(); })
}

module.exports =
    {
        get: get,
        set: set
    };
