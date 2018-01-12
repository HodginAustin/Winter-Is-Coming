#include "./includes/DailyState.hpp"

class Schedule
{
private:
    unsigned int id;
    DailyState* weeklyState[7];

public:
    Schedule(unsigned int id);

    // Accessors
    unsigned int get_id();
    DailyState* get_daily_state(unsigned int day);

    // Mutators
    void set_id(unsigned int);
    void set_daily_state(unsigned int day, DailyState* state);
};
