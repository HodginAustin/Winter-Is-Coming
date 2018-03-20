/******************************************************
* Plant.js - Travis Hodgin,
* Gets everything needed to render the main plant page
******************************************************/

module.exports = function () {
  var express = require('express');
  var router = express.Router();
  const http = require("http");

  /* Connection settings */
  let conn = require('./global/connection.js');
  
  router.get('/', function (req, res) {
    var context = req.context;

    res.render('plant', context);
  });


  return router;
}();
