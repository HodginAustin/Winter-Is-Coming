/* Get LED states */
function getAllLEDStates(res, context, complete) {
    var conn = require('./connection.js');
    const http = require("http");

    var uri = conn.url + conn.port + '/' + conn.ledStates;

    http.get(uri, res => {
        res.setEncoding("utf8");
        body = "";
        res.on("data", data => {
            body += data;
        });
        res.on("end", () => {
            body = JSON.parse(body);
            context.LEDStates = body;
            complete();
        });
    });
}

module.exports = 
{
    get: getAllLEDStates
};