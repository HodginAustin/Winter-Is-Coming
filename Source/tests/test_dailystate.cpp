#include <unordered_map>

#include "./asserting.h"
#include "../includes/DailyState.hpp"
#include "../includes/LEDState.hpp"

bool test_dailystate()
{
    DailyState* d = new DailyState(24);
    LEDState* lds = new LEDState(1);
    std::unordered_map<unsigned int, LEDState*> time_state_map;

    bool condTrue = true;
    bool condFalse = false;
    
    // ID
    condTrue &= assertTrue(d->get_id() == 24);

    // time<->state map empty initially
    condTrue &= assertTrue(d->get_time_state_map() == time_state_map);

    // Add state to map
    // state happens 12 seconds after midnight, and LED state has ID 1
    time_state_map.insert(std::make_pair(12, lds));
    d->add_state(12, lds);
    condTrue &= assertTrue(d->get_time_state_map() == time_state_map);

    // Get existing LED state and confirm match
    condTrue &= assertTrue(d->get_led_state(12) == lds);

    // Get non-existent LED state and confirm match the most recent LED state (12)
    condTrue &= assertTrue(d->get_led_state(13) == lds);

    // Delete non-existent LED state and confirm that the previous state still exists
    d->delete_state(1500);
    condTrue &= assertTrue(d->get_led_state(12) == lds);

    // Delete existing LED state and confirm it's gone
    bool deleteResult = d->delete_state(12);
    condTrue &= assertTrue(deleteResult);

    time_state_map.erase(12);

    condTrue &= assertTrue(d->get_time_state_map() == time_state_map);
    condTrue &= assertTrue(d->get_time_state_count() == 0);
    

    //condFalse |= assertFalse();

    return condTrue && !condFalse;
}