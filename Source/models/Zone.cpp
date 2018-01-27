#include <algorithm>

#include "../includes/Zone.hpp"


// Constructor
Zone::Zone() {}
Zone::Zone(const Zone& z)
{
    copy(z);
}


// Copy
void Zone::copy(const Zone& z)
{
    if (z.get_schedule() != 0){
        set_schedule(z.get_schedule());
    }
    if (!z.get_name().empty()){
        set_name(z.get_name());
    }

    std::vector<LED*> leds = z.get_leds();
    if (!leds.empty()) {
        for (std::vector<LED*>::iterator iter = leds.begin(); iter < leds.end(); iter++){
            add_led(*iter);
        }
    }
}


// Accessors
unsigned int Zone::get_id() const
{
    return id;
}

std::string Zone::get_name() const
{
    return name;
}

Schedule* Zone::get_schedule() const
{
    return schedule;
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

void Zone::set_schedule(Schedule* newSchedule)
{
    schedule = newSchedule;
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

void Zone::delete_led(LED* led)
{
    leds.erase(std::remove(leds.begin(), leds.end(), led), leds.end());
}


// JSON
void to_json(json& j, const Zone& z) {
    // Build JSON from led vector
    json leds_j = json::array(); // Empty JSON array []
    std::vector<LED*> leds = z.get_leds();
    for (std::vector<LED*>::iterator iter = leds.begin(); iter < leds.end(); iter++){
        json led_j;
        led_j["id"] = (*iter)->get_id();
        leds_j.push_back(led_j);
    }
    
    j = json{
        {"id", z.get_id()},
        {"name", z.get_name()},
        {"schedule", z.get_schedule()->get_id()},
        {"leds", leds_j}
    };
}

void from_json(const json& j, Zone& z) {
    z.set_name(j.at("name").get<std::string>());
    //p.set_schedule(j.at("schedule").get<std::string>());
    //p.add_led(j.at("description").get<std::string>());
}