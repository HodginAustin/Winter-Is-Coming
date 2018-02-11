// Include Guard
#ifndef __ZONE_H_INCLUDED__
#define __ZONE_H_INCLUDED__
//=================================


#include <vector>
#include <string>

#include "./json.hpp"

#include "./LED.hpp"
#include "./DailyState.hpp"
#include "./LEDState.hpp"

using nlohmann::json;

class Zone
{
private:
    unsigned int id;
    std::string name;
    std::array<DailyState*, 7> weeklyState;
    std::vector<LED*> leds;

public:
    // USE ACCESSORS AND MUTATORS
    unsigned int profile_id;

    // Constructor
    Zone();
    Zone(const Zone& z);

    // Copy
    void copy(const Zone& z);

    // Accessors
    const unsigned int& get_id() const;
    const std::string& get_name() const;

    // Mutators
    void set_id(unsigned int);
    void set_name(std::string value);

    // CRUD
    void add_led(LED* led);
    std::vector<LED*> get_leds() const;
    bool has_led(LED* led) const;
    void delete_led(LED* led);

    // Scheduling
    DailyState* get_daily_state(unsigned int day) const;
    LEDState* get_active_state(unsigned int time, int day) const;

    // Mutators
    void set_daily_state(unsigned int day, DailyState* state);
};

// JSON
void to_json(json& j, const Zone& p);
void from_json(const json& j, Zone& p);


#endif //__ZONE_H_INCLUDED__
