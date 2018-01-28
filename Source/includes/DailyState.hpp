// Include Guard
#ifndef __DAILYSTATE_H_INCLUDED__
#define __DAILYSTATE_H_INCLUDED__
//=================================

#include "./json.hpp"

#include <unordered_map>
#include "./LEDState.hpp"

using nlohmann::json;

class DailyState
{
private:
    unsigned int id;
    std::unordered_map<unsigned int, LEDState*> timeStatePairs;

public:
    // Constructor
    DailyState();
    DailyState(const DailyState& d);

    // Copy
    void copy(const DailyState& d);

    // Accessors
    unsigned int get_id() const;
    std::unordered_map<unsigned int, LEDState*> get_time_state_map() const;

    // Mutators
    void set_id(unsigned int);

    // CRUD
    bool add_state(unsigned int time, LEDState* state);
    std::unordered_map<unsigned int, LEDState*> get_led_states() const;
    LEDState* get_led_state(unsigned int time);
    bool delete_state(unsigned int time);
    int get_time_state_count() const;
};

// JSON
void to_json(json& j, const DailyState& ds);
void from_json(const json& j, DailyState& ds);


#endif //__DAILYSTATE_H_INCLUDED__
