#include "../includes/Schedule.hpp"

Schedule::Schedule(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int Schedule::get_id()
{
    return id;
}

unsigned int Schedule::get_effective_time()
{
    return effective_time;
}

unsigned int Schedule::get_led_state_id()
{
    return led_state_id;
}


// Mutators
void Schedule::set_id(unsigned int val)
{
    id = val;
}

void Schedule::set_effective_time(unsigned int val)
{
    effective_time = val;
}

void Schedule::set_led_state_id(unsigned int val)
{
    led_state_id = val;
}
