#include <algorithm>
#include "../includes/Zone.hpp"


// Constructor
Zone::Zone(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int Zone::get_id()
{
    return id;
}

Schedule* Zone::get_schedule()
{
    return schedule;
}


// Mutators
void Zone::set_id(unsigned int val)
{
    id = val;
}

void Zone::set_schedule(Schedule* newSchedule)
{
    schedule = newSchedule;
}


// CRUD
void Zone::add_led(LED* led)
{
    leds.push_back(led);
}

std::vector<LED*> Zone::get_leds()
{
    return leds;
}

void Zone::delete_led(LED* led)
{
    leds.erase(std::remove(leds.begin(), leds.end(), led), leds.end());
}
