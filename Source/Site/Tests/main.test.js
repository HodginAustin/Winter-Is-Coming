var express = require('express'); /*express for handlebars*/
const path = require("path"); /* allows use of path.join for folder paths */
const http = require('http'); /* make internal requests to control service */

var app = express();
/* Current profile for navbar */
let currentProfile = require('../js/global/currentProfile.js');

/* Profiles for navbar */
let profiles = require('../js/global/profiles.js');

/* Connection settings */
let conn = require('../js/global/connection.js');


// Files and data
app.use('../js', express.static(path.join(__dirname, 'js'))); /* Javascript folder */
app.use('../css', express.static(path.join(__dirname, 'css'))); /* style sheets folder */
app.use('../img', express.static(path.join(__dirname, 'img'))); /* images folder */


app.set('port', process.argv[2]); /* sets port to what is given in command line */


// Global call
app.all('*', function(req, res, next) {
  var callbackCount = 0;
  let context = {};

  if (req.method === 'GET') {
    /* Get current profile */
    currentProfile.get(res, context, gotCurrentProfile);

    function gotCurrentProfile() {
      callbackCount++;

      /* Get profiles */
      profiles.get(res, context, complete);
    }

  } else {
    callbackCount = 100;
    complete();
  }

  function complete() {
    callbackCount++;
    if (callbackCount >= 2) {
      req.context = context;
      next();
    }
  }
});

// Set current profile
app.post('/currentProfile/', function(req, res){
  let id = req.body.id;
  currentProfile.set(res, id, function(){
    res.redirect('back');
  });
});

// Base pages
app.get('/', function(req, res){
  res.redirect('/plant');
});
app.use(require('../js/plant.js'));
app.use(require('../js/hardware.js'));
app.use(require('../js/profiles.js'));
app.use(require('../js/ledStates.js'));
app.use(require('../js/dailyStates.js'));
app.use(require('../js/settings.js'));

// Listen
app.listen(app.get('port'), function () {
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});
