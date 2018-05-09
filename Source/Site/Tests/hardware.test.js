let conn = require('../js/global/connection.js');
let controlService = require('../js/global/controlServiceRequest.js');
var chai = require('chai');
var supertest = require('supertest');
var cheerio = require('cheerio');
var assert = require('assert');

describe("Hardware", function(){
  var request = supertest('http://localhost:8080');

  it('should render hardware page', function(done){
    var should = require('should');
    request.get('/hardware')
    .expect(200).end(function(err, res){
      done();
    });
  });

  it('should display IO port 1', function(done){
      var should = require('should');
      request.get('/hardware', function(err, response, html){
        const $ = cheerio.load(html);
      done();
      });
  });


});
