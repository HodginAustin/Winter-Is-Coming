/* Get Daily states */
function getAllDailyStates(res, context, complete) {
    let conn = require('./connection.js');
    const http = require("http");

    /* Control service request */
    let controlService = require('./controlServiceRequest.js');
    
    // Control service options
    let options = {
        method: "GET",
        uri: conn.url + conn.port + '/' + conn.dailyStates,
        json: {}
    };

    // Make request
    controlService.makeRequest(options, function (err, response, body) {
        context.DailyStates = body;
        complete();
    });
}

module.exports =
    {
        get: getAllDailyStates
    };