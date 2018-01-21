#include <algorithm>
#include "./includes/InternalState.hpp"


// Required for static class members
// Abstract relationship objects (also zones and schedules inside profiles)
std::vector<Profile*> InternalState::profiles;
Profile* InternalState::currentProfile;
// Physically based objects
std::vector<LED*> InternalState::leds;
std::vector<Controller*> InternalState::controllers;
// State objects
std::vector<LEDState*> InternalState::ledStates;
std::vector<DailyState*> InternalState::dailyStates;


// Initialization
void InternalState::Initialize()
{
    currentProfile = 0;
}


// Accessors
Profile* InternalState::get_current_profile()
{
    return currentProfile;
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

void InternalState::delete_profile(Profile* profile)
{
    profiles.erase(
        std::remove(profiles.begin(), profiles.end(), profile), profiles.end());
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

void InternalState::delete_led(LED* led)
{
    leds.erase(
        std::remove(leds.begin(), leds.end(), led), leds.end());
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

void InternalState::delete_controller(Controller* controller)
{
    controllers.erase(
        std::remove(controllers.begin(), controllers.end(), controller), controllers.end());
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

void InternalState::delete_led_state(LEDState* ledState)
{
    ledStates.erase(
        std::remove(ledStates.begin(), ledStates.end(), ledState), ledStates.end());
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

void InternalState::delete_daily_state(DailyState* dailyState)
{
    dailyStates.erase(
        std::remove(dailyStates.begin(), dailyStates.end(), dailyState), dailyStates.end());
}
