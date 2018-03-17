/* Get Daily states */
function getAllDailyStates(res, context, complete) {
    var conn = require('./connection.js');
    const http = require("http");

    var uri = conn.url + conn.port + '/' + conn.dailyStates;

    http.get(uri, res => {
        res.setEncoding("utf8");
        body = "";
        res.on("data", data => {
            body += data;
        });
        res.on("end", () => {
            body = JSON.parse(body);
            context.DailyStates = body;
            complete();
        });
    });
}

module.exports =
    {
        get: getAllDailyStates
    };