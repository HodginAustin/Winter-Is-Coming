/*Make a request to the control service */
function controlServiceRequest(options, callback) {
    var request = require('request');

    request(options, function (err, response, body) {
        if (err || response.statusCode != 200) {
            let error = "Error! Status: " + response.statusCode + ", Response: " + String(response.body);
            console.log(error);
            console.log(response.body);
        }
        else {
            console.log("Control Service Response:" + response.body);
            callback(err, response, body);
        }
    });
}


module.exports =
    {
        makeRequest: controlServiceRequest
    };