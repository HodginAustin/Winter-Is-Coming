#include <vector>

#include "../asserting.h"
#include "../../includes/Zone.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/DailyState.hpp"

bool test_zone()
{
    Zone* z = new Zone();
    z->set_id(9);
    LED* led1 = new LED();
    led1->set_id(1);
    LED* led2 = new LED();
    led2->set_id(2);
    LED* led3 = new LED();
    led3->set_id(3);

    bool condTrue = true;
    bool condFalse = false;

    // ID
    condTrue &= assertTrue(z->get_id() == 9);

    // leds vector empty initially
    condTrue &= assertTrue(z->get_leds().size()==0);

    // Add led to zone
    z->add_led(led1);
    condTrue &= assertTrue(z->get_leds().size()==1);
    condTrue &= assertTrue(z->get_leds()[0]==led1);


    // Delete non-existent led and confirm that count does not change
    z->delete_led(led3);
    condTrue &= assertTrue(z->get_leds().size()==1);

    // Add second led to zone
    z->add_led(led2);
    condTrue &= assertTrue(z->get_leds()[1]==led2);
    condTrue &= assertTrue(z->get_leds().size()==2);

    // Delete existing leds
    z->delete_led(led1);
    condTrue &= assertTrue(z->get_leds().size()==1);
    z->delete_led(led2);
    condTrue &= assertTrue(z->get_leds().size()==0);


    // Scheduling tests
    LEDState* l1 = new LEDState();
    l1->set_id(1);
    LEDState* l2 = new LEDState();
    l2->set_id(2);
    LEDState* l3 = new LEDState();
    l3->set_id(3);

    DailyState* d1 = new DailyState();
    d1->set_id(1);
    d1->add_state(61200, l3); // 5PM LED state l3
    DailyState* d2 = new DailyState();
    d2->set_id(2);
    d2->add_state(28800, l1); // 8AM LED state l1
    d2->add_state(30600, l2); // 8:30AM LED state l2
    DailyState* d3 = new DailyState();
    d3->set_id(3);


    // DailyState list empty initially
    condTrue &= assertTrue(z->get_daily_state(0) == 0);
    condTrue &= assertTrue(z->get_daily_state(3) == 0);
    condTrue &= assertTrue(z->get_daily_state(6) == 0);

    // Set daily states
    z->set_daily_state(6, d1); // Saturday state d1
    z->set_daily_state(0, d2); // Sunday state d2
    z->set_daily_state(3, d3); // Wednesday state d3
    condTrue &= assertTrue(z->get_daily_state(0)->get_id() == 2);
    condTrue &= assertTrue(z->get_daily_state(3)->get_id() == 3);
    condTrue &= assertTrue(z->get_daily_state(6)->get_id() == 1);

    // Check active state
    // State at 8:00:15AM on sunday should be l1
    condTrue &= assertTrue(z->get_active_state(28815, 0) == l1);
    // State at 8:29:59AM on sunday should be l1
    condTrue &= assertTrue(z->get_active_state(30599, 0) == l1);
    // State at 8:30:00AM on sunday should be l2
    condTrue &= assertTrue(z->get_active_state(30600, 0) == l2);
    // State at 5:00PM on sunday should still be l2
    condTrue &= assertTrue(z->get_active_state(61200, 0) == l2);
    // State at midnight on monday should still be l2
    condTrue &= assertTrue(z->get_active_state(0, 1) == l2);
    // State at 7:00AM on Sunday should be the last state on Saturday (l3)
    condTrue &= assertTrue(z->get_active_state(25200, 0) == l3);


    return condTrue && !condFalse;
}
