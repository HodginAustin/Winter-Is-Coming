/*gets current profile. Uses async to collect data and complete to render
THIS FUNCTION SHOULD BE INCLUDED IN ALL PAGES FOR THE NAVBAR CURRENT PROFILE*/
function get(res, context, complete) {
    var conn = require('./connection.js');
    const http = require("http");

    var current = conn.url + conn.port + '/' + conn.currentProfile;
    http.get(current, res => {
        res.setEncoding("utf8");
        body = "";
        res.on("data", data => {
            body += data;
        });
        res.on("end", () => {
            body = JSON.parse(body);
            context.currentProfile = body;
            complete();
        });
    });
}

function set(res, context, complete) {

}

module.exports = 
{
    get: get,
    set: set
};