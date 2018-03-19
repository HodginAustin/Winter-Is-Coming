/* Get controllers */
function getAllControllers(res, context, complete) {
    let conn = require('./connection.js');
    const http = require("http");

    /* Control service request */
    let controlService = require('./controlServiceRequest.js');
    
    // Control service options
    let options = {
        method: "GET",
        uri: conn.url + conn.port + '/' + conn.controllers,
        json: {}
    };

    // Make request
    controlService.makeRequest(options, function (err, response, body) {
        context.Controllers = body;
        complete();
    });
}

module.exports =
    {
        get: getAllControllers
    };