/******************************************************
 * simulator.js - Zach Lerew
 * Gets everything needed to render the hardware simulator
 ******************************************************/

module.exports = function() {
    var express = require('express');
    var router = express.Router();

    // Internal variables
    var beds = [];

    // API - takes data in to display
    router.post('/update/:io/:r/:g/:b/:strip_idx', function(req, res) {
        
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
                res.status(200).send("ACK");
                return;
            }

            res.status(400).send("BAD IO or STRIP_IDX");
            return;
        }
        res.status(400).send("Malformed URL");
        return;
    });

    function maxBedId() {
        let m = 0;
        beds.forEach(function(b) {
            m = Math.max(b.id, m);
        });
        return m;
    }

    router.post('/putBed/', function(req, res) {
        let bed = {};
        let f = beds.filter(b => b.id == req.query.bedId);
        if (req.query.bedId && f.length > 0) {
            bed = f[0];
            bed.leds = [];
        } else {
            let m = maxBedId()+1;
            beds.push({ "id": m, "leds": [] });
            bed = beds.filter(b => b.id == m)[0];
        }
        if (req.body.leds) {
            for (let i = 0; i < parseInt(req.body.leds); i++) {
                bed.leds.push({ r: 0, g: 0, b: 0, strip_idx: i });
            }

            res.status(200).json({ id: bed.id });
            return;
        }
        res.status(400).send("Bad Body");
    });

    router.delete('/deleteBed/:bedId', function(req, res) {
        if (req.params.bedId) {
            if (beds.filter(b => b.id == req.params.bedId).length > 0) {
                beds = beds.filter(b => b.id != req.params.bedId);

                res.status(201).send("success");
                return;
            }
            res.status(404).send("BedID not found");
            return;
        }
        res.status(404).send("BedID not given");
    });


    // Simulator page page
    router.get('/', function(req, res) {
        var context = { beds: beds.filter(b => b != null) };

        res.status(200);
        res.render('simulator', context);
    });

    router.get('/getBeds', function(req, res) {
        let context = { layout: false, beds: beds.filter(b => b != null) };

        res.status(200);
        res.render('simulator_beds', context);
    });

    return router;
}();