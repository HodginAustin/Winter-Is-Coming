var chai = require('chai');
var profilesJS = require('../js/profiles.js');
let conn = require('../js/global/connection.js');
let controlService = require('../js/global/controlServiceRequest.js');

var i = 1;

describe("Profiles", function(){
  beforeEach(function(done){
    let j = {};
    j['name'] = "Test Name";
    j['description'] = "Test Description";
    var options = {
        method: "POST",
        uri: conn.url + conn.port + "/" + conn.profiles + "/add",
        json: j
    };
    controlService.makeRequest(options, function () {});

  });

  afterEach(function(done){
    var id = i;
    var options = {
        method: "DELETE",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/delete"
    };
    controlService.makeRequest(options, function () {});
     i++;
  });

  it('should return all profiles', function(done){
    request(require('../main.js'))
      .get('/profiles')
      .expect(200, function(err, res){
       expect(res.body.name).to.equal("Test Name");
          done();
      });
  });

  it('get single profile should return status 200', function(done){

  });
  it('edit profile should return status 200', function(done){

  });
  it('delete profile should return status 200', function(done){

  });
  it('should add a profile and give status 200', function(done){

  });

});
