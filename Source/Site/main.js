var express = require('express'); /*express for handlebars*/
var handlebars = require('express-handlebars'); /* templating engine */
var bodyParser = require('body-parser');
var tinycolor = require("tinycolor2");
const path = require("path"); /* allows use of path.join for folder paths */
const http = require('http'); /* make internal requests to control service */

var app = express();
/* Current profile for navbar */
let currentProfile = require('./js/global/currentProfile.js');

/* Profiles for navbar */
let profiles = require('./js/global/profiles.js');

/* Connection settings */
let conn = require('./js/global/connection.js');

/* sets port to what is given in command line */
app.set('port', process.argv[2]);


// Handlebars
var hbs = handlebars.create({
    helpers: {
        getLEDStateFromID: function (state, ledStates) {
            let found = ledStates.find(function(s) {
                return s.id == state;
            });

        return "<a class='badge' style='border: 1px solid black; background-color: rgb("+found.r+", "+found.g+", "+found.b+")'>ID: " + state + "</a>";
        },
    },
    defaultLayout: 'main'
});

app.engine('handlebars', hbs.engine);
app.set('view engine', 'handlebars');


// Files and data
app.use(bodyParser.urlencoded({ extended: true }));

app.use('/static', express.static(path.join(__dirname, 'public'))); /* public folder */
app.use('/js', express.static(path.join(__dirname, 'js'))); /* Javascript folder */
app.use('/css', express.static(path.join(__dirname, 'css'))); /* style sheets folder */
app.use('/img', express.static(path.join(__dirname, 'img'))); /* images folder */


// Global call
app.all('*', function (req, res, next) {
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
app.post('/currentProfile/', function (req, res) {
    let id = req.body.id;
    currentProfile.set(res, id, function () {
        res.redirect('back');
    });
});

// Base pages
app.get('/', function (req, res) {
    res.redirect('/plant');
});
app.use('/plant', require('./js/plant.js'));
app.use('/hardware', require('./js/hardware.js'));
app.use('/profiles', require('./js/profiles.js'));
app.use('/ledStates', require('./js/ledStates.js'));
app.use('/dailyStates', require('./js/dailyStates.js'));
app.use('/settings', require('./js/settings.js'));
app.use('/simulator', require('./js/simulator.js'));


// Bad states
app.use(function (req, res) {
    res.status(404);
    res.render('404');
});

app.use(function (req, res) {
    res.status(500);
    res.render('500');
});


// Listen
app.listen(app.get('port'), function () {
    console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});
