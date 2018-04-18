var express = require('express'); /*express for handlebars*/
var handlebars = require('express-handlebars'); /* templating engine */
var bodyParser = require('body-parser');

var app = express();

app.engine('handlebars', handlebars({
    defaultLayout: 'main',
}));


// Internal variables
var beds = [];


// Handlebars
app.set('view engine', 'handlebars');
app.set('port', process.argv[2]); /* sets port to what is given in command line */

var bodyParser = require('body-parser')
app.use(bodyParser.json()); // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({ // to support URL-encoded bodies
    extended: true
}));


// API - takes data in to display
app.post('/update/:io/:r/:g/:b/:strip_idx', function(req, res) {
    let io = parseInt(req.params.io);
    let r = parseInt(req.params.r);
    let g = parseInt(req.params.g);
    let b = parseInt(req.params.b);
    let strip_idx = parseInt(req.params.strip_idx);

    if (io >= 0 && r >= 0 && g >= 0 && b >= 0 && strip_idx >= 0) {
        if (beds[io - 1] && beds[io - 1].leds[strip_idx]) {
            beds[io - 1].leds[strip_idx].r = r;
            beds[io - 1].leds[strip_idx].g = g;
            beds[io - 1].leds[strip_idx].b = b;
            console.log(beds[io - 1]);
            res.status(200).send("ACK");
            return;
        }

        res.status(400).send("BAD IO or STRIP_IDX");
        return;
    }
    res.status(400).send("Malformed URL");
    return;
});

app.post('/postBed', function(req, res) {
    if (req.body.leds) {
        beds.push({ "id": beds.length, "leds": [] });
        for (let i = 0; i < parseInt(req.body.leds); i++) {
            beds[beds.length - 1]["leds"].push({ r: 0, g: 0, b: 0, strip_idx: i });
        }

        res.status(200).json({ id: beds.length - 1 });
        return;
    }
    res.status(400).send("Bad Body");
});

app.delete('/deleteBed/:bedId', function(req, res) {
    if (req.params.bedId) {
        if (beds[req.params.bedId]) {
            beds[req.params.bedId] = null;
            res.status(201).send("success");
        }
        res.status(404).send("BedID not found");
        return;
    }
    res.status(404).send("BedID not given");
});


// Simulator page page
app.get('/', function(req, res) {
    var context = { beds: beds.filter(b => b != null) };

    res.status(200);
    res.render('beds', context);
});


// Bad states
app.use(function(req, res) {
    res.status(404);
    res.send('404');
});

app.use(function(req, res) {
    res.status(500);
    res.send('500');
});


// Listen
app.listen(app.get('port'), function() {
    console.log('LED Simulator started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});