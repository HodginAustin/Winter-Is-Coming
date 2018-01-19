#include <vector>

#include "./asserting.h"
#include "../includes/Zone.hpp"

bool test_zone()
{
    Zone* z = new Zone(9);
    LED* led1 = new LED(1);
    LED* led2 = new LED(2);
    LED* led3 = new LED(3);

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


    return condTrue && !condFalse;
}