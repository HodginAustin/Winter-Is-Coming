#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"

#include <iostream>
bool test_delete()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static void remove(Profile*);
        static void remove(Zone*);
        static void remove(LED*);
        static void remove(LEDState*);
        static void remove(DailyState*);
        static void remove(Controller*);
        static void remove(Zone*, LED*);
        static void remove(Zone*, int);
        static void remove(DailyState*, int);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    condTrue &= assertTrue(DataParser::selectProfile(1) == 0);
    unsigned int pid = DataParser::insert(p);
    p->set_id(pid);
    condTrue &= assertTrue(DataParser::selectProfile(pid) != 0);
   

    // ZONE
    Zone* z = new Zone();
    z->set_profile_id(p->get_id());
    condTrue &= assertTrue(DataParser::selectZone(1) == 0);
    unsigned int zid = DataParser::insert(z);
    z->set_id(zid);
    condTrue &= assertTrue(DataParser::selectZone(zid) != 0);


    // CONTROLLER
    Controller* c = new Controller();
    condTrue &= assertTrue(DataParser::selectController(1) == 0);
    unsigned int cid = DataParser::insert(c);
    c->set_id(cid);
    condTrue &= assertTrue(DataParser::selectController(cid) != 0);


    // LED
    LED* l = new LED();
    l->set_controller(c);
    condTrue &= assertTrue(DataParser::selectLED(1) == 0);
    unsigned int lid = DataParser::insert(l);
    l->set_id(lid);
    condTrue &= assertTrue(DataParser::selectLED(lid) != 0);


    // DAILY STATE
    DailyState* ds = new DailyState();
    condTrue &= assertTrue(DataParser::selectDailyState(1) == 0);
    unsigned int dsid = DataParser::insert(ds);
    ds->set_id(dsid);
    condTrue &= assertTrue(DataParser::selectDailyState(dsid) != 0);


    // LED STATE
    LEDState* ls = new LEDState();
    condTrue &= assertTrue(DataParser::selectLEDState(1) == 0);
    unsigned int lsid = DataParser::insert(ls);
    ls->set_id(lsid);
    condTrue &= assertTrue(DataParser::selectLEDState(lsid) != 0);
    

    // Zone DOW
    ZoneDOW zoneDow;
    zoneDow.daily_state_id = ds->get_id();
    zoneDow.day_of_week = 2;
    zoneDow.zone_id = z->get_id();
    condTrue &= assertTrue(DataParser::selectZoneDOW(1).zone_id == 0);
    DataParser::insert(zoneDow);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).zone_id != 0);


    // ZoneToLED
    ZoneToLED zoneToLED;
    zoneToLED.zone_id = z->get_id();
    zoneToLED.led_id = l->get_id();
    condTrue &= assertTrue(DataParser::selectZoneToLED(1).zone_id == 0);
    DataParser::insert(zoneToLED);
    condTrue &= assertTrue(DataParser::selectZoneToLED(z->get_id()).zone_id != 0);


    // DailyStateToLEDState
    DailyStateToLEDState dailyStateToLEDState;
    dailyStateToLEDState.daily_state_id = ds->get_id();
    dailyStateToLEDState.time = 2000;
    dailyStateToLEDState.led_state_id = l->get_id();
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(1).daily_state_id == 0);
    DataParser::insert(dailyStateToLEDState);
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(z->get_id()).daily_state_id != 0);


    // Delete data (done separately from insert due to FK constraints)
    DataParser::remove(z, l);
    condTrue &= assertTrue(DataParser::selectZoneToLED(z->get_id()).zone_id == 0);

    DataParser::remove(z, 2);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).zone_id == 0);

    DataParser::remove(l);
    condTrue &= assertTrue(DataParser::selectZoneDOW(z->get_id()).zone_id == 0);

    DataParser::remove(ds, 2000);
    condTrue &= assertTrue(DataParser::selectDailyStateToLEDState(ds->get_id()).daily_state_id == 0);

    DataParser::remove(c);
    condTrue &= assertTrue(DataParser::selectController(cid) == 0);

    DataParser::remove(z);
    condTrue &= assertTrue(DataParser::selectZone(zid) == 0);
   
    DataParser::remove(p);
    condTrue &= assertTrue(DataParser::selectProfile(pid) == 0);


    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
