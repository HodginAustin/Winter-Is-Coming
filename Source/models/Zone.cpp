#include <algorithm>

#include "../includes/Zone.hpp"


// Constructor
Zone::Zone()
{
    name = "";
    weeklyState = {0,0,0,0,0,0,0};
}
Zone::Zone(const Zone& z)
{
    copy(z);
}


// Copy
void Zone::copy(const Zone& z)
{
    if (!z.get_name().empty()){
        set_name(z.get_name());
    }

    std::vector<LED*> leds = z.get_leds();
    if (!leds.empty()) {
        for (auto led : leds) {
            add_led(led);
        }
    }

    set_profile_id(z.get_profile_id());

    set_daily_state(0, z.get_daily_state(0));
    set_daily_state(1, z.get_daily_state(1));
    set_daily_state(2, z.get_daily_state(2));
    set_daily_state(3, z.get_daily_state(3));
    set_daily_state(4, z.get_daily_state(4));
    set_daily_state(5, z.get_daily_state(5));
    set_daily_state(6, z.get_daily_state(6));
}


// Accessors
const unsigned int& Zone::get_id() const
{
    return this->id;
}

const std::string& Zone::get_name() const
{
    return this->name;
}

const unsigned int& Zone::get_profile_id() const
{
    return profile_id;
}


// Mutators
void Zone::set_id(unsigned int val)
{
    id = val;
}

void Zone::set_name(std::string val)
{
    name = val;
}

void Zone::set_profile_id(unsigned int id)
{
    profile_id = id;
}


// CRUD
void Zone::add_led(LED* led)
{
    leds.push_back(led);
}

std::vector<LED*> Zone::get_leds() const
{
    return leds;
}

bool Zone::has_led(LED* led) const
{
    return std::find(leds.begin(), leds.end(), led) != leds.end();
}

void Zone::delete_led(LED* led)
{
    leds.erase(std::remove(leds.begin(), leds.end(), led), leds.end());
}


// Scheduling
DailyState* Zone::get_daily_state(unsigned int day) const
{
    if (day < 0 || day > 6) { return 0; }
    return weeklyState.at(day);
}

LEDState* get_led_state_from_daily_state(unsigned int time, DailyState* ds)
{
    if (!ds) { return 0; }
    return ds->get_led_state(time);
}

LEDState* Zone::get_active_state(unsigned int time_of_day, int day) const
{
    if (day < 0 || day > 6) { return 0; }
    if (time_of_day > 24*60*60) { return 0; }

    int dayToCheck = day;
    unsigned int timeToCheck = time_of_day;

    // Try to get closest active state initially
    DailyState* ds = weeklyState.at(dayToCheck);

    LEDState* ls = get_led_state_from_daily_state(timeToCheck, ds);

    // If the initial day does not exist or it has no daily states
    while (ls == 0) {

        // Look to the previous day
        dayToCheck--;
        // If we had to look back at the previous day, set the time to the end of the day
        timeToCheck = 24 * 60 * 60;

        // Loop backwards from sunday to saturday
        if (dayToCheck < 0) { dayToCheck = 6; }
        // If we loop backwards all the way to the day we started at, return null
        if (dayToCheck == day) { break; }

        // Get the previous day of the week
        ds = weeklyState.at(dayToCheck);
        // Try to get the latest LED state
        ls = get_led_state_from_daily_state(timeToCheck, ds);
    }
    
    if (ls == 0) {
        return LEDState::off;
    } else {
        return ls;
    }
}

void Zone::set_daily_state(unsigned int day, DailyState* state)
{
    if (day < 0 || day > 6) { return; }

    weeklyState[day] = state;
}


// JSON
void to_json(json& j, const Zone& z) {
    // Build JSON from led vector
    json leds_j = json::array(); // Empty JSON array []
    std::vector<LED*> leds = z.get_leds();
    for (auto led : leds) {
        leds_j.push_back(led->get_id());
    }

    // Build JSON for all 7 days
    json days_j = json::array(); // Empty JSON array []
    for (int i = 0; i < 7; i++){
        DailyState* day = z.get_daily_state(i);
        json day_j;
        if (day) { days_j.push_back(day->get_id()); }
        else { days_j.push_back(0); }
    }
        
    j = json{
        {"id", z.get_id()},
        {"name", z.get_name()},
        {"days", days_j},
        {"leds", leds_j}
    };
}

void from_json(const json& j, Zone& z) {
    if (j.find("name") != j.end()){
        z.set_name(j.at("name").get<std::string>());
    }
}
