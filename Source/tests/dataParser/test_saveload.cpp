#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"
#include "../../includes/InternalState.hpp"
#include "../../includes/Settings.hpp"

#include <iostream>
bool test_saveload()
{
    // Initialize
    InternalState::initialize();
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static bool get_all();
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    p->set_name("ptest");
    p->set_description("ptestdesc");
    p->set_id(DataParser::insert(p));

    // ZONE
    Zone* z = new Zone();
    z->set_name("zone1");
    z->set_profile_id(p->get_id());
    z->set_id(DataParser::insert(z));

    // CONTROLLER
    Controller* c = new Controller();
    c->set_address("addr");
    c->set_details("details");
    c->set_io(3);
    c->set_id(DataParser::insert(c));
    
    // LED
    LED* l = new LED();
    l->set_strip_idx(12);
    l->set_controller(c);
    l->set_id(DataParser::insert(l));

    LED* l2 = new LED();
    l2->set_strip_idx(2);
    l2->set_controller(c);
    l2->set_id(DataParser::insert(l2));

    // DAILY STATE
    DailyState* ds = new DailyState();
    ds->set_id(DataParser::insert(ds));

    // LED STATE
    LEDState* ls = new LEDState();
    ls->set_r(201);
    ls->set_g(202);
    ls->set_b(203);
    ls->set_intensity(20);
    ls->set_power(false);
    ls->set_id(DataParser::insert(ls));
    
    // SETTINGS
    Setting st;
    st.name = "test";
    st.int_value = 4;
    DataParser::insert(st);

    // Zone DOW
    ZoneDOW zoneDow;
    zoneDow.daily_state_id = ds->get_id();
    zoneDow.day_of_week = 1;
    zoneDow.zone_id = z->get_id();
    DataParser::insert(zoneDow);

    // ZoneToLED
    ZoneToLED zoneToLED;
    zoneToLED.zone_id = z->get_id();
    zoneToLED.led_id = l->get_id();
    DataParser::insert(zoneToLED);

    // DailyStateToLEDState
    DailyStateToLEDState dailyStateToLEDState;
    dailyStateToLEDState.daily_state_id = ds->get_id();
    dailyStateToLEDState.time = 2002;
    dailyStateToLEDState.led_state_id = ls->get_id();
    DataParser::insert(dailyStateToLEDState);
    

    // Load data into internal state
    DataParser::get_all();


    // Check internal state data
    condTrue &= assertTrue(InternalState::get_profile(p->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_profile(p->get_id())->get_zone(z->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_controller(c->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_led(l->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_led(l2->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_daily_state(ds->get_id()) != 0);
    condTrue &= assertTrue(InternalState::get_led_state(ls->get_id()) != 0);
    condTrue &= assertTrue(Settings::get_setting(st.name).int_value == 4);
    condTrue &= assertTrue(InternalState::get_profile(p->get_id())->get_zone(z->get_id())->get_daily_state(zoneDow.day_of_week) != 0);
    condTrue &= assertTrue(InternalState::get_profile(p->get_id())->get_zone(z->get_id())->get_leds()[0]->get_id() == l->get_id());
    condTrue &= assertTrue(InternalState::get_daily_state(ds->get_id())->get_led_state(dailyStateToLEDState.time)->get_id() == ls->get_id());


    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
