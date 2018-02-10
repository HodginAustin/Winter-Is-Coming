#include <iterator>
#include "../includes/DailyState.hpp"
#include "../includes/InternalState.hpp"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Constructor
DailyState::DailyState() {}
DailyState::DailyState(const DailyState& d)
{
    copy(d);
}


// Copy
void DailyState::copy(const DailyState& d)
{
    std::unordered_map<unsigned int, LEDState*> states = d.get_time_state_map();
    if (!states.empty()) {
        // Iterate through all map pairs
        for (auto& element : states) {
            unsigned int t = element.first;
            LEDState* s = element.second;
            add_state(t, s);
        }
    }
}


// Accessors
const unsigned int& DailyState::get_id() const
{
    return this->id;
}

std::unordered_map<unsigned int, LEDState*> DailyState::get_time_state_map() const
{
    return timeStatePairs;
}


// Mutators
void DailyState::set_id(unsigned int val)
{
    id = val;
}


// CRUD
bool DailyState::add_state(unsigned int time, LEDState* state)
{
    // Check if key already exists, update if it does, insert new key if not
    if (timeStatePairs.count(time) > 0) {
        return (timeStatePairs[time] = state);
    } else {
        return timeStatePairs.insert(std::make_pair(time, state)).second;
    }
}

std::unordered_map<unsigned int, LEDState*> DailyState::get_led_states() const
{
    return timeStatePairs;
}

LEDState* DailyState::get_led_state(unsigned int time_of_day)
{
    LEDState* nearest_state = 0;
    unsigned int nearest_time = 0;

    // First check if no time<->state pairs exist
    if (timeStatePairs.count(time_of_day) == 0) {
        if (timeStatePairs.size() == 0) {
            return 0;
        }

        for (auto& element : timeStatePairs) {
            unsigned int t = element.first;

            // Only check times greater than or equal to the given time
            if (time_of_day >= t) {
                // Find the greatest of the times listed for this day
                nearest_time = MAX(t, nearest_time);
            }
        }
        nearest_state = timeStatePairs[nearest_time];
        return nearest_state;
    } else {
        return timeStatePairs[time_of_day];
    }
}

bool DailyState::delete_state(unsigned int time)
{
    timeStatePairs.erase(time);
    return timeStatePairs.count(time) == 0;
}

int DailyState::get_time_state_count() const
{
    return timeStatePairs.size();
}

void DailyState::clear_led_states()
{
    timeStatePairs.clear();
}


// JSON
void to_json(json& j, const DailyState& ds)
{
    // Build JSON from timeState map
    json tsm_j = json::array(); // Empty JSON array []
    std::unordered_map<unsigned int, LEDState*> timeStateMap = ds.get_time_state_map();

    // Iterate through all map pairs
    for (auto& element : timeStateMap) {
        unsigned int t = element.first;
        LEDState* s = element.second;

        json ts_j = json{
            {"time", t},
            {"state", s->get_id()}
        };
        tsm_j.push_back(ts_j);
    }

    j = json{
        {"id", ds.get_id()},
        {"timeStateMap", tsm_j},
    };
}

void from_json(const json& j, DailyState& ds)
{
    if (j.find("timeStateMap") != j.end()) {
        // Clear old map
        ds.clear_led_states();

        json tsm_j = j.at("timeStateMap").get<json>();
        for (auto& element : tsm_j) {
            json ts_j = element;
            LEDState* ls = InternalState::get_led_state(ts_j["state"]);
            ds.add_state(ts_j["time"], ls);
        }
    }
}
