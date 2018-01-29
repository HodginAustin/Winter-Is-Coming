// Include Guard
#ifndef __SCHEDULE_H_INCLUDED__
#define __SCHEDULE_H_INCLUDED__
//=================================

#include <array>

#include "./json.hpp"

#include "./DailyState.hpp"
#include "./LEDState.hpp"

using nlohmann::json;

class Schedule
{
private:
    unsigned int id;
    std::array<DailyState*, 7> weeklyState;

public:
    // Constructor
    Schedule();
    Schedule(const Schedule& s);

    // Copy
    void copy(const Schedule& s);

    // Accessors
    unsigned int get_id() const;
    DailyState* get_daily_state(unsigned int day) const;
    LEDState* get_active_state(unsigned int time, int day) const;

    // Mutators
    void set_id(unsigned int);
    void set_daily_state(unsigned int day, DailyState* state);
};

// JSON
void to_json(json& j, const Schedule& s);

#endif //__SCHEDULE_H_INCLUDED__
