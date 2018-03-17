var express = require('express'); /*express for handlebars*/
var handlebars = require('express-handlebars'); /* templating engine */
var bodyParser = require('body-parser');
const path = require("path"); /* allows use of path.join for folder paths */
const http = require('http'); /* make internal requests to control service */

var app = express();

/* Connection settings */
let conn = require('./js/global/connection.js');

app.engine('handlebars', handlebars({
  defaultLayout: 'main',
}));


// Files and data
app.use(bodyParser.urlencoded({ extended: true }));

app.use('/static', express.static(path.join(__dirname, 'public'))); /* public folder */
app.use('/js', express.static(path.join(__dirname, 'js'))); /* Javascript folder */
app.use('/css', express.static(path.join(__dirname, 'css'))); /* style sheets folder */
app.use('/img', express.static(path.join(__dirname, 'img'))); /* images folder */


// Handlebars
app.set('view engine', 'handlebars');
app.set('port', process.argv[2]); /* sets port to what is given in command line */


// Base pages
app.get('/', function(req, res){
  res.redirect('/plant');
});
app.use('/plant', require('./js/plant.js'));
app.use('/profiles', require('./js/profiles.js'));
app.use('/zones', require('./js/zones.js'));
app.use('/ledStates', require('./js/ledStates.js'));
app.use('/dailyStates', require('./js/dailyStates.js'));


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
