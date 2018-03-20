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
        
        // Add a current flag to the current profile
        for (let p = 0; p < context.Profiles.length; p++) {
            if (context.Profiles[p].id == context.currentProfile.id) {
                context.Profiles[p].active = true;
            } else {
                context.Profiles[p].active = false;
            }
        }

        complete();
    });
}

module.exports = 
{
    get: getAllProfiles
};