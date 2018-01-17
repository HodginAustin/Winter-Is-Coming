#include "../includes/Schedule.hpp"


// Constructor
Schedule::Schedule(unsigned int id)
{
    set_id(id);
    set_daily_state(0, 0);
    set_daily_state(1, 0);
    set_daily_state(2, 0);
    set_daily_state(3, 0);
    set_daily_state(4, 0);
    set_daily_state(5, 0);
    set_daily_state(6, 0);
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

LEDState* get_led_state_from_daily_state(unsigned int time, DailyState* ds)
{
    if (ds == 0) { return 0; }
    return ds->get_led_state(time);
}

LEDState* Schedule::get_active_state(unsigned int time, int day)
{
    if (day < 0 || day > 6) { return 0; }
    if (time < 0 || time > 24*60*60) { return 0; }

    int dayToCheck = day;
    unsigned int timeToCheck = time;

    // Try to get closest active state initially
    DailyState* ds = weeklyState[dayToCheck];
    LEDState* ls = get_led_state_from_daily_state(timeToCheck, ds);

    // If the initial day does not exist or it has no daily states
    while (ds == 0 || ls == 0){
        // Look to the previous day
        dayToCheck--;
        // If we had to look back at the previous day, set the time to the end of the day
        timeToCheck = 24 * 60 * 60;
        // Loop backwards from sunday to saturday
        if (dayToCheck < 0) { dayToCheck = 6; }
        // If we loop backwards all the way to the day we started at, return null
        if (dayToCheck == day) { return 0; }

        // Get the previous day of the week
        ds = weeklyState[dayToCheck];
        // Try to get the latest LED state
        ls = get_led_state_from_daily_state(timeToCheck, ds);
    }

    return ls;
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
