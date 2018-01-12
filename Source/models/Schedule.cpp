#include "../includes/Schedule.hpp"


// Constructor
Schedule::Schedule(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int Schedule::get_id()
{
    return id;
}

DailyState* Schedule::get_daily_state(unsigned int day)
{
    if (day < 0 || day > 6) { return 0; }

    return weeklyState[day];
}


// Mutators
void Schedule::set_id(unsigned int val)
{
    id = val;
}

void Schedule::set_daily_state(unsigned int day, DailyState* state)
{
    if (day < 0 || day > 6 || state == 0) { return; }

    weeklyState[day] = state;
}
