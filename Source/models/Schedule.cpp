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

LEDState* Schedule::get_active_state(unsigned int time, unsigned int day)
{
    if (day < 0 || day > 6) { return 0; }
    if (time < 0 || time > 24*60*60) { return 0; }

    DailyState* ds = weeklyState[day];
    if (ds == 0) { return 0; }

    return ds->get_led_state(time);
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
