#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"

#include <iostream>
bool test_update()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static void update(Profile*);
        static void update(Zone*);
        static void update(LED*);
        static void update(LEDState*);
        static void update(DailyState*);
        static void update(Controller*);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    p->set_name("ptest");
    condTrue &= assertTrue(DataParser::selectProfile(1) == 0);
    unsigned int pid = DataParser::insert(p);
    p->set_id(pid);
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_name() == p->get_name());
    p->set_name("ptest2");
    DataParser::update(p);
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_name() == p->get_name());


    // ZONE
    Zone* z = new Zone();
    z->set_profile_id(pid);
    z->set_name("ztest");
    condTrue &= assertTrue(DataParser::selectZone(1) == 0);
    unsigned int zid = DataParser::insert(z);
    z->set_id(zid);
    condTrue &= assertTrue(DataParser::selectZone(zid)->get_name() == z->get_name());
    z->set_name("ztest2");
    DataParser::update(z);
    condTrue &= assertTrue(DataParser::selectZone(zid)->get_name() == z->get_name());


    // CONTROLLER
    Controller* c = new Controller();
    c->set_io(3);
    condTrue &= assertTrue(DataParser::selectController(1) == 0);
    unsigned int cid = DataParser::insert(c);
    c->set_id(cid);
    condTrue &= assertTrue(DataParser::selectController(cid)->get_io() == c->get_io());
    c->set_io(4);
    DataParser::update(c);
    condTrue &= assertTrue(DataParser::selectController(cid)->get_io() == c->get_io());


    // LED
    LED* l = new LED();
    l->set_controller(c);
    condTrue &= assertTrue(DataParser::selectLED(1) == 0);
    unsigned int lid = DataParser::insert(l);
    l->set_id(lid);
    condTrue &= assertTrue(DataParser::selectLED(lid)->get_controller_id() == c->get_id());

    Controller* c2 = new Controller();
    c2->set_id(DataParser::insert(c2));
    l->set_controller(c2);

    DataParser::update(l);
    condTrue &= assertTrue(DataParser::selectLED(lid)->get_controller_id() == c2->get_id());


    // DAILY STATE (no tests, has no DB properties, all fields covered by relationship objects)


    // LED STATE
    LEDState* ls = new LEDState();
    ls->set_r(1);
    ls->set_g(1);
    ls->set_b(1);
    ls->set_intensity(1);
    ls->set_power(true);
    condTrue &= assertTrue(DataParser::selectLEDState(1) == 0);
    unsigned int lsid = DataParser::insert(ls);
    ls->set_id(lsid);
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_r() == ls->get_r());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_g() == ls->get_g());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_b() == ls->get_b());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_intensity() == ls->get_intensity());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_power() == ls->get_power());

    ls->set_r(2);
    ls->set_g(3);
    ls->set_b(4);
    ls->set_intensity(14);
    ls->set_power(false);

    DataParser::update(ls);
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_r() == ls->get_r());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_g() == ls->get_g());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_b() == ls->get_b());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_intensity() == ls->get_intensity());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_power() == ls->get_power());


    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
