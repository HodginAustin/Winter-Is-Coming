  /* connections and  requests services */
  var conn = require('../js/global/connection.js');
  var controlService = require('../js/global/controlServiceRequest.js');

function NukeAll(){
  var options = {
      method: "DELETE",
      uri: conn.url + conn.port + "/nuke_from_orbit",
      json: {}
  };

  // Make request
  controlService.makeRequest(options, function() {});
}

  /****************************************************************/
  /* setup Test hardware                                         */
  /****************************************************************/
  function addTestController(complete){
    let j = {};
    j['io'] = 1;
    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.controllers + "/add",
      json: j
    };

    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    complete();
  }

  function addTestLeds(complete){
    let controllerID = 1;
    let nextIdx = 0;
    let count = 60;
    j = [];
    for(let i = 0; i < count; i++){
      let t = {};
      t['controller'] = parseInt(controllerID);
      t['strip_idx'] = parseInt(nextIdx + i);
      j.push(t);
    }
    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.leds + "/add",
      json: j
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    complete();
  }
  /****************************************************************/

  /****************************************************************/
  /* setup Test LED states                                             */
  /****************************************************************/
  function addTestLEDStates(complete){
    let j = {};
    j['r'] = 255;
    j['g'] = 255;
    j['b'] = 255;
    j['intensity'] = 100;
    j['power'] = true;

    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.ledStates + "/add",
      json: j
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    complete();
  }
  /****************************************************************/

  /****************************************************************/
  /* setup Test DailyStates                                       */
  /****************************************************************/
  function addTestDailyStates(complete){
    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.dailyStates + "/add",
      json: {}
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    /* setup the LED states for this daily state */
    var j = {};
    j['r'] = 255;
    j['g'] = 255;
    j['b'] = 255;
    j['intensity'] = 100;
    j['power'] = true;
    var options2 = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.dailyStates + "/1" + "/edit",
        json: j
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    var j2 = {};
    j2['time'] = 28800; /* 8:00 am */
    j2['state'] = 1; /* on */
    j2 = [j2]
    var options3 = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.dailyStates + "/1" + "/led_states/add",
        json: j
    };
    controlService.makeRequest(options3, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    var j3 = {};
    j3['time'] = 61200; /* 5:00 pm */
    j3['state'] = 1; /* on */
    j3 = [j3]
    var options4 = {
        method: "PUT",
        uri: conn.url + conn.port + "/" + conn.dailyStates + "/1" + "/led_states/add",
        json: j3
    };
    controlService.makeRequest(options4, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    complete();
  }
  /****************************************************************/


  /****************************************************************/
  /* setup Test Profiles
  /****************************************************************/
  function addTestProfiles(complete){
    /* setup profile */
    var j = {};
    j['name'] = "TESTING NAME 1";
    j['description'] = "THIS IS FOR TESTING!";
    var options = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.profiles + "/add",
      json: j
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    /* setup zones */
    var j2 = {};
    j2['name'] = "TEST ZONE";
    var options2 = {
      method: "POST",
      uri: conn.url + conn.port + "/" + conn.profiles + "/1" + "/zones/add",
      json: j2
    };
    controlService.makeRequest(options2, function (err, response, body) {
      if(err){
        console.log(err);
      }}
    );
    /* add leds to zones */
    let min = 0;
    let max = 60;
    let range = max - min;
    let j3 = [];
    for(let i = 0; i < range+1; i++){
      j3.push(min + i);
    }
    var options = {
      method: "PUT",
      uri: conn.url + conn.port + "/" + conn.profiles + "/1" + "/zones/1" + "/leds/add",
      json: j3
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    /* set days */
    let days = {};
    days["sunday"] = 1;
    days["monday"] = 1;
    days["tuesday"] = 1;
    days["wednesday"] = 1;
    days["thursday"] = 1;
    days["friday"] = 1;
    days["saturday"] = 1;
    var options = {
      method: "PUT",
      uri: conn.url + conn.port + "/" + conn.profiles + "/1" + "/zones/1" + "/days/add/",
      json: j
    };
    controlService.makeRequest(options, function (err, response, body) {
      if(err){
        console.log(err);
      }
    });
    complete();
  }

  /****************************************************************/

module.exports = {
  addTestController: addTestController,
  addTestLeds: addTestLeds,
  addTestDailyStates: addTestDailyStates,
  addTestLEDStates: addTestLEDStates,
  addTestProfiles: addTestProfiles,
  NukeAll: NukeAll
}
