#include <iterator>
#include "../includes/DailyState.hpp"

// Constructor
DailyState::DailyState(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int DailyState::get_id()
{
    return id;
}

std::map<unsigned int, LEDState*> DailyState::get_hourly_state_map()
{
    return hourlyStates;
}


// Mutators
void DailyState::set_id(unsigned int val)
{
    id = val;
}


// CRUD
bool DailyState::add_state(unsigned int time, LEDState* state)
{
    // Check if key already exists, update if it does, insert new key if not
    if (hourlyStates.count(time) > 0) {
        return (hourlyStates[time] = state);
    } else {
        return hourlyStates.insert(std::make_pair(time, state)).second;
    }
}

LEDState* DailyState::get_led_state(unsigned int time)
{
    if (hourlyStates.count(time) > 0) {
        return hourlyStates[time];
    } else {
        return 0;
    }
}

bool DailyState::delete_state(unsigned int time)
{
    std::map<unsigned int, LEDState*>::iterator iter = hourlyStates.find(time);
    if (iter == hourlyStates.end()) { return false; }

    hourlyStates.erase(iter);
    return true;
}
