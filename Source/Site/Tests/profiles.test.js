var profilesJS = require('../js/profiles.js');
let conn = require('../js/global/connection.js');
let controlService = require('../js/global/controlServiceRequest.js');
var chai = require('chai');
var sinon = require('sinon');
var express = require('express');
var supertest = require('supertest');



var i = 1;

describe("Profiles", function(){
  var app, request, route, getProfileStub;
  beforeEach(function(done){
    let j = {};
    app = express();
    route = proxyquire('../js/profiles.js', {
      get
    });
    route(app);
    request = supertest(app);
    j['name'] = "Test Name";
    j['description'] = "Test Description";
    var options = {
        method: "POST",
        uri: conn.url + conn.port + "/" + conn.profiles + "/add",
        json: j
    };
    controlService.makeRequest(options, function () {});
    done();
  });

  afterEach(function(done){
    var id = i;
    var options = {
        method: "DELETE",
        uri: conn.url + conn.port + "/" + conn.profiles + "/" + id + "/delete"
    };
    controlService.makeRequest(options, function () {});
     done();
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
    done();

  });
  it('edit profile should return status 200', function(done){
    done();

  });
  it('delete profile should return status 200', function(done){
    done();

  });
  it('should add a profile and give status 200', function(done){
    done();
  });

});
