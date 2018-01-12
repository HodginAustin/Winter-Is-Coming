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

std::map<unsigned int, unsigned int> DailyState::get_hourly_state_map()
{
    return hourlyStates;
}


// Mutators
void DailyState::set_id(unsigned int val)
{
    id = val;
}


// CRUD
bool DailyState::create_update_state(unsigned int time, unsigned int state)
{
    // Check if key already exists, update if it does, insert new key if not
    if (hourlyStates.count(time) > 0) {
        hourlyStates[time] = state;
        return false;
    } else {
        std::pair<std::map<unsigned int, unsigned int>::iterator, bool> result;
        result = hourlyStates.insert(std::pair<unsigned int, unsigned int>(time, state));
        return result.second;
    }
}

unsigned int DailyState::get_led_state_id(unsigned int time)
{
    if (hourlyStates.count(time) > 0) {
        return hourlyStates[time];
    } else {
        return 0;
    }
}

bool DailyState::delete_state(unsigned int time)
{
    std::map<unsigned int, unsigned int>::iterator iter = hourlyStates.find(time);
    if (iter == hourlyStates.end()) { return false; }

    hourlyStates.erase(iter);
    return true;
}
