#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"
#include "../../includes/Setting.hpp"
#include "../../includes/relationships.hpp"

#include <iostream>
bool test_insert()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static unsigned int insert(Profile*);
        static unsigned int insert(Zone*);
        static unsigned int insert(LED*);
        static unsigned int insert(LEDState*);
        static unsigned int insert(DailyState*);
        static unsigned int insert(Controller*);
        static void insert(ZoneDOW);
        static void insert(ZoneToLED);
        static void insert(DailyStateToLEDState);
        static void insert(Setting setting);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    p->set_name("ptest");
    p->set_description("ptestdesc");
    condTrue &= assertTrue(DataParser::selectProfile(1) == 0);
    unsigned int pid = DataParser::insert(p);
    p->set_id(pid);
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_id() == p->get_id());
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_name() == p->get_name());
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_description() == p->get_description());

    Profile* p2 = new Profile();
    condTrue &= assertTrue(DataParser::selectProfile(1) != 0);
    unsigned int pid2 = DataParser::insert(p2);
    p2->set_id(pid2);
    condTrue &= assertTrue(DataParser::selectProfile(pid2) != DataParser::selectProfile(pid));
    

    // ZONE
    Zone* z = new Zone();
    z->set_name("zone1");
    z->set_profile_id(p->get_id());
    condTrue &= assertTrue(DataParser::selectZone(1) == 0);
    unsigned int zid = DataParser::insert(z);
    z->set_id(zid);
    condTrue &= assertTrue(DataParser::selectZone(zid)->get_id() == z->get_id());
    condTrue &= assertTrue(DataParser::selectZone(zid)->get_name() == z->get_name());

    Zone* z2 = new Zone();
    z2->set_profile_id(p2->get_id());
    condTrue &= assertTrue(DataParser::selectZone(1) != 0);
    unsigned int zid2 = DataParser::insert(z2);
    z2->set_id(zid2);
    condTrue &= assertTrue(DataParser::selectZone(zid2) != DataParser::selectZone(zid));


    // CONTROLLER
    Controller* c = new Controller();
    c->set_address("addr");
    c->set_details("details");
    c->set_io(3);
    condTrue &= assertTrue(DataParser::selectController(1) == 0);
    unsigned int cid = DataParser::insert(c);
    c->set_id(cid);
    condTrue &= assertTrue(DataParser::selectController(cid)->get_id() == c->get_id());
    condTrue &= assertTrue(DataParser::selectController(cid)->get_address() == c->get_address());
    condTrue &= assertTrue(DataParser::selectController(cid)->get_details() == c->get_details());
    condTrue &= assertTrue(DataParser::selectController(cid)->get_io() == c->get_io());

    Controller* c2 = new Controller();
    condTrue &= assertTrue(DataParser::selectController(1) != 0);
    unsigned int cid2 = DataParser::insert(c2);
    c2->set_id(cid2);
    condTrue &= assertTrue(DataParser::selectController(cid2) != DataParser::selectController(cid));


    // LED
    LED* l = new LED();
    l->set_strip_idx(12);
    l->set_controller(c);
    condTrue &= assertTrue(DataParser::selectLED(1) == 0);
    unsigned int lid = DataParser::insert(l);
    l->set_id(lid);
    condTrue &= assertTrue(DataParser::selectLED(lid)->get_id() == l->get_id());
    condTrue &= assertTrue(DataParser::selectLED(lid)->get_strip_idx() == l->get_strip_idx());

    LED* l2 = new LED();
    l2->set_controller(c2);
    condTrue &= assertTrue(DataParser::selectLED(1) != 0);
    unsigned int lid2 = DataParser::insert(l2);
    l2->set_id(lid2);
    condTrue &= assertTrue(DataParser::selectLED(lid2) != DataParser::selectLED(lid));


    // DAILY STATE
    DailyState* ds = new DailyState();
    condTrue &= assertTrue(DataParser::selectDailyState(1) == 0);
    unsigned int dsid = DataParser::insert(ds);
    ds->set_id(dsid);
    condTrue &= assertTrue(DataParser::selectDailyState(dsid)->get_id() == ds->get_id());

    DailyState* ds2 = new DailyState();
    condTrue &= assertTrue(DataParser::selectDailyState(1) != 0);
    unsigned int dsid2 = DataParser::insert(ds2);
    ds2->set_id(dsid2);
    condTrue &= assertTrue(DataParser::selectDailyState(dsid2) != DataParser::selectDailyState(dsid));


    // LED STATE
    LEDState* ls = new LEDState();
    ls->set_r(201);
    ls->set_g(202);
    ls->set_b(203);
    ls->set_intensity(20);
    ls->set_power(false);
    condTrue &= assertTrue(DataParser::selectLEDState(1) == 0);
    unsigned int lsid = DataParser::insert(ls);
    ls->set_id(lsid);
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_id() == ls->get_id());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_r() == ls->get_r());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_g() == ls->get_g());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_b() == ls->get_b());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_intensity() == ls->get_intensity());
    condTrue &= assertTrue(DataParser::selectLEDState(lsid)->get_power() == ls->get_power());

    LEDState* ls2 = new LEDState();
    condTrue &= assertTrue(DataParser::selectLEDState(1) != 0);
    unsigned int lsid2 = DataParser::insert(ls2);
    ls2->set_id(lsid2);
    condTrue &= assertTrue(DataParser::selectLEDState(lsid2) != DataParser::selectLEDState(lsid));

    
    // SETTINGS
    Setting st;
    st.name = "test";
    st.int_value = 4;
    condTrue &= assertTrue(DataParser::selectSetting("test").name == "");
    DataParser::insert(st);
    condTrue &= assertTrue(DataParser::selectSetting("test").name == "test");
    condTrue &= assertTrue(DataParser::selectSetting("test").int_value == 4);


    // Zone DOW
    ZoneDOW zoneDow;
    zoneDow.daily_state_id = ds->get_id();
    zoneDow.day_of_week = 1;
    zoneDow.zone_id = z->get_id();
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).zone_id == 0);
    DataParser::insert(zoneDow);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).zone_id == zoneDow.zone_id);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).day_of_week == zoneDow.day_of_week);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).daily_state_id == zoneDow.daily_state_id);


    // ZoneToLED
    ZoneToLED zoneToLED;
    zoneToLED.zone_id = z->get_id();
    zoneToLED.led_id = l->get_id();
    condTrue &= assertTrue(DataParser::selectZoneToLED(z->get_id()).zone_id == 0);
    DataParser::insert(zoneToLED);
    condTrue &= assertTrue(DataParser::selectZoneToLED(z->get_id()).zone_id == zoneToLED.zone_id);
    condTrue &= assertTrue(DataParser::selectZoneToLED(z->get_id()).led_id == zoneToLED.led_id);


    // DailyStateToLEDState
    DailyStateToLEDState dailyStateToLEDState;
    dailyStateToLEDState.daily_state_id = ds->get_id();
    dailyStateToLEDState.time = 2002;
    dailyStateToLEDState.led_state_id = ls->get_id();
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(ds->get_id()).daily_state_id == 0);
    DataParser::insert(dailyStateToLEDState);
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(ds->get_id()).daily_state_id == dailyStateToLEDState.daily_state_id);
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(ds->get_id()).time == dailyStateToLEDState.time);
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(ds->get_id()).led_state_id == dailyStateToLEDState.led_state_id);


    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
