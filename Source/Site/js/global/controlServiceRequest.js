/*Make a request to the control service */
function controlServiceRequest(options, callback) {
    console.log(JSON.stringify(options));
    let request = require('request');

    request(options, function (err, response, body) {
        if (response) {
            console.log(options.method + " " + options.uri + " " + JSON.stringify(options.json));
            if (err || response.statusCode != 200) {
                let error = "    ERROR: " + response.statusCode + " " + JSON.stringify(response.body);
                console.log(error);
            }
            else {
                console.log("    " + JSON.stringify(response.body).slice(0, 100));
            }
            callback(err, response, body);
        }
        else {
            callback(err, null, "");
        }
    });
}


module.exports =
    {
        makeRequest: controlServiceRequest
    };