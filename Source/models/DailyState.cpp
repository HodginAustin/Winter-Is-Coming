#include "../includes/DailyState.hpp"

DailyState::DailyState(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int DailyState::get_id()
{
    return id;
}

unsigned int DailyState::get_effective_time()
{
    return effective_time;
}

unsigned int DailyState::get_led_state_id()
{
    return led_state_id;
}


// Mutators
void DailyState::set_id(unsigned int val)
{
    id = val;
}

void DailyState::set_effective_time(unsigned int val)
{
    effective_time = val;
}

void DailyState::set_led_state_id(unsigned int val)
{
    led_state_id = val;
}
