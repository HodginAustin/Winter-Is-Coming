#include "../includes/DailyState.hpp"
#include "../includes/InternalState.hpp"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Constructor
DailyState::DailyState() {
    other = 0;
}
DailyState::DailyState(const DailyState& d)
{
    other = 0;
    copy(d);
}


// Copy
void DailyState::copy(const DailyState& d)
{
    // Clear existing led statess
    clear_led_states();

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
    unsigned int nearest_time = 0;

    // If there are no mappings then return null
    if (timeStatePairs.size() == 0) {
        return 0;
    }

    std::unordered_map<unsigned int, LEDState*>::const_iterator found = timeStatePairs.find(time_of_day);

    // Check if no time<->state pairs exist
    if (found == timeStatePairs.end()) {
        
        for (auto& element : timeStatePairs) {
            unsigned int t = element.first;

            // Only check times less than or equal to the given time
            if (t <= time_of_day) {
                // Find the greatest of the times listed for this day
                nearest_time = MAX(t, nearest_time);
                found = timeStatePairs.find(nearest_time);
            }
        }

        // Lookup LED state
        found = timeStatePairs.find(nearest_time);
        if (found != timeStatePairs.end()) {
            if (found->second == 0) {
                return LEDState::off;
            } else {
                return found->second;
            }
        } else {
            return 0;
        }
    } else {
        return timeStatePairs[time_of_day];
    }
}

bool DailyState::delete_time_state(unsigned int time)
{
    timeStatePairs.erase(time);
    return timeStatePairs.count(time) == 0;
}

void DailyState::delete_led_state(LEDState* ledState)
{
    std::vector<unsigned int> toDelete;

    for (auto& element : timeStatePairs) {
        if (element.second != 0) {
            if (element.second->get_id() == ledState->get_id()) {
                toDelete.push_back(element.first);
            }
        }
    }

    for (auto& e : toDelete) {
        delete_time_state(e);
    }
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
        unsigned int stateID = (s ? s->get_id() : 0);


        json ts_j = json{
            {"time", t},
            {"state", stateID}
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
    // Clear old map
    ds.clear_led_states();

    for (auto& element : j) {
        json ts_j = element;
        LEDState* ls = InternalState::get_led_state(ts_j["state"]);
        unsigned int time_of_day = ts_j["time"];
        if (time_of_day >= 0 && time_of_day <= 24*60*60) {
            ds.add_state(time_of_day, ls);
        }
    }
}
