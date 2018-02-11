#include <algorithm>
#include <ctime>

#include "./includes/InternalState.hpp"


// Required for static class members
// Abstract relationship objects (also zones inside profiles)
std::vector<Profile*> InternalState::profiles;
Profile* InternalState::currentProfile;
// Physically based objects
std::vector<LED*> InternalState::leds;
std::vector<Controller*> InternalState::controllers;
// State objects
std::vector<LEDState*> InternalState::ledStates;
std::vector<DailyState*> InternalState::dailyStates;


// Initialization
bool InternalState::initialize()
{
    currentProfile = 0;
    return true;
}


// Accessors
Profile* InternalState::get_current_profile()
{
    return currentProfile;
}


// Mutators
void InternalState::set_current_profile(Profile* p)
{
    currentProfile = p;
}


// Profile CRUD
void InternalState::add_profile(Profile* profile)
{
    profiles.push_back(profile);
}

std::vector<Profile*> InternalState::get_profiles()
{
    return profiles;
}

Profile* InternalState::get_profile(unsigned int id)
{
    for (auto profile : profiles) {
        if (profile->get_id() == id){ return profile; }
    }
    return 0;
}

void InternalState::delete_profile(Profile* profile)
{
    profiles.erase(
        std::remove(profiles.begin(), profiles.end(), profile), profiles.end());

    free(profile);
}


// LED CRUD
void InternalState::add_led(LED* led)
{
    leds.push_back(led);
}

std::vector<LED*> InternalState::get_leds()
{
    return leds;
}

LED* InternalState::get_led(unsigned int id)
{
    for (auto led : leds) {
        if (led->get_id() == id){ return led; }
    }
    return 0;
}

void InternalState::delete_led(LED* led)
{
    leds.erase(
        std::remove(leds.begin(), leds.end(), led), leds.end());

    free(led);
}


// Controller CRUD
void InternalState::add_controller(Controller* controller)
{
    controllers.push_back(controller);
}

std::vector<Controller*> InternalState::get_controllers()
{
    return controllers;
}

Controller* InternalState::get_controller(unsigned int id)
{
    for (auto controller : controllers) {
        if (controller->get_id() == id){ return controller; }
    }
    return 0;
}

void InternalState::delete_controller(Controller* controller)
{
    controllers.erase(
        std::remove(controllers.begin(), controllers.end(), controller), controllers.end());

    free(controller);
}


// LEDState CRUD
void InternalState::add_led_state(LEDState* ledState)
{
    ledStates.push_back(ledState);
}

std::vector<LEDState*> InternalState::get_led_states()
{
    return ledStates;
}

LEDState* InternalState::get_led_state(unsigned int id)
{
    for (auto ledState : ledStates) {
        if (ledState->get_id() == id){ return ledState; }
    }
    return 0;
}

void InternalState::delete_led_state(LEDState* ledState)
{
    ledStates.erase(
        std::remove(ledStates.begin(), ledStates.end(), ledState), ledStates.end());

    free(ledState);
}


// DailyState CRUD
void InternalState::add_daily_state(DailyState* dailyState)
{
    dailyStates.push_back(dailyState);
}

std::vector<DailyState*> InternalState::get_daily_states()
{
    return dailyStates;
}

DailyState* InternalState::get_daily_state(unsigned int id)
{
    for (auto dailyState : dailyStates) {
        if (dailyState->get_id() == id){ return dailyState; }
    }
    return 0;
}

void InternalState::delete_daily_state(DailyState* dailyState)
{
    dailyStates.erase(
        std::remove(dailyStates.begin(), dailyStates.end(), dailyState), dailyStates.end());
}


// Clear
void InternalState::clear()
{
    // Profiles
    for (auto profile : profiles)
    {
        for (auto zone : profile->get_zones())
        {
            free(zone);
        }
        free(profile);
    }
    profiles.clear();

    currentProfile = 0;

    // Physically based objects
    for (auto led : leds)
    {
        free(led);
    }
    leds.clear();

    for (auto controller : controllers)
    {
        free(controller);
    }
    controllers.clear();

    // State objects
    for (auto ledState : ledStates)
    {
        free(ledState);
    }
    ledStates.clear();

    for (auto dailyState : dailyStates)
    {
        free(dailyState);
    }
    dailyStates.clear();
}


// Helpers
unsigned int InternalState::get_time()
{
    time_t sysTime;
    time(&sysTime);

    tm* timeInfo = localtime(&sysTime);
    return (timeInfo->tm_hour * 3600) + (timeInfo->tm_min * 60) + (timeInfo->tm_sec);
}

int InternalState::get_day()
{
    time_t sysTime;
    time(&sysTime);
    
    tm* timeInfo = localtime(&sysTime);
    return timeInfo->tm_wday;
}
