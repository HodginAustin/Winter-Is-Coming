// Include Guard
#ifndef __SCHEDULE_H_INCLUDED__
#define __SCHEDULE_H_INCLUDED__
//=================================


#include "./DailyState.hpp"
#include "./LEDState.hpp"

class Schedule
{
private:
    unsigned int id;
    DailyState* weeklyState[7];

public:
    // Constructor
    Schedule(unsigned int id);

    // Accessors
    unsigned int get_id();
    DailyState* get_daily_state(unsigned int day);
    LEDState* get_active_state(unsigned int time, int day);

    // Mutators
    void set_id(unsigned int);
    void set_daily_state(unsigned int day, DailyState* state);
};

#endif //__SCHEDULE_H_INCLUDED__
