let conn = require('./connection.js');

function getAllZones(res, context, id, complete){
  var zoneURL = conn.url + conn.port + '/' + conn.profiles + '/' + id + '/zones';
  let controlService = require('./controlServiceRequest.js')

  let options = {
    method: "GET",
    uri: zoneURL,
    json: {}
  };

  controlService.makeRequest(options, function(err, response, body){
    context.Zones = body;
    complete();
  });
}

module.exports =
  {
    get: getAllZones
  };
