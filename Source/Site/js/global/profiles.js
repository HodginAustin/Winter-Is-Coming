/* Get profiles */
function getAllProfiles(res, context, complete) {
    let conn = require('./connection.js');
    const http = require("http");

    /* Control service request */
    let controlService = require('./controlServiceRequest.js');
    
    // Control service options
    let options = {
        method: "GET",
        uri: conn.url + conn.port + '/' + conn.profiles,
        json: {}
    };

    // Make request
    controlService.makeRequest(options, function (err, response, body) {
        context.Profiles = body;
        complete();
    });
}

module.exports = 
{
    get: getAllProfiles
};