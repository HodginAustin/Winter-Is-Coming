// Include Guard
#ifndef __DAILYSTATE_H_INCLUDED__
#define __DAILYSTATE_H_INCLUDED__
//=================================


#include <unordered_map>
#include "./LEDState.hpp"

class DailyState
{
private:
    unsigned int id;
    std::unordered_map<unsigned int, LEDState*> timeStatePairs;

public:
    // Constructor
    DailyState(unsigned int id);

    // Accessors
    unsigned int get_id();
    std::unordered_map<unsigned int, LEDState*> get_time_state_map();

    // Mutators
    void set_id(unsigned int);

    // CRUD
    bool add_state(unsigned int time, LEDState* state);
    LEDState* get_led_state(unsigned int time);
    bool delete_state(unsigned int time);
    int get_time_state_count();
};

#endif //__DAILYSTATE_H_INCLUDED__
