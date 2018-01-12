#include "../includes/LED.hpp"


// Constructor
LED::LED(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int LED::get_id()
{
    return id;
}

unsigned int LED::get_strip_idx()
{
    return strip_idx;
}

unsigned int LED::get_controller_idx()
{
    return controller_idx;
}

unsigned int LED::get_zone_id()
{
    return zone_id;
}

unsigned int LED::get_profile_id()
{
    return profile_id;
}


// Mutators
void LED::set_id(unsigned int val)
{
    id = val;
}

void LED::set_strip_idx(unsigned int val)
{
    strip_idx = val;
}

void LED::set_controller_idx(unsigned int val)
{
    controller_idx = val;
}

void LED::set_zone_id(unsigned int val)
{
    zone_id = val;
}

void LED::set_profile_id(unsigned int val)
{
    profile_id = val;
}
