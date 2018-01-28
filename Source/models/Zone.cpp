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
        for (auto led : leds) {
            add_led(led);
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
    for (auto led : leds) {
        json led_j;
        led_j["id"] = led->get_id();
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
    if (j.find("name") != j.end()){
        z.set_name(j.at("name").get<std::string>());
    }
}
