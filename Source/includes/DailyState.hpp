// Include Guard
#ifndef __DAILYSTATE_H_INCLUDED__
#define __DAILYSTATE_H_INCLUDED__
//=================================


#include <map>
#include "./LEDState.hpp"

class DailyState
{
private:
    unsigned int id;
    std::map<unsigned int, LEDState*> hourlyStates;

public:
    // Constructor
    DailyState(unsigned int id);

    // Accessors
    unsigned int get_id();
    std::map<unsigned int, LEDState*> get_hourly_state_map();

    // Mutators
    void set_id(unsigned int);

    // CRUD
    /// Summary ///
    // Returns true when key/value is added or updated, false when key exists
    ///
    bool add_state(unsigned int time, LEDState* state);
    LEDState* get_led_state(unsigned int time);
    bool delete_state(unsigned int time);
};

#endif //__DAILYSTATE_H_INCLUDED__
