#include "./includes/StateComposer.hpp"


// Required for static class members
bool StateComposer::logEnable;
char StateComposer::composerState;

time_t StateComposer::sysTime;
int StateComposer::weekDay;
Profile* StateComposer::currentProfile;
Schedule* StateComposer::currentZoneSchedule;
LEDState* StateComposer::currentZoneActiveState;
std::vector<LED*> StateComposer::currentZoneLEDs;
Controller* StateComposer::currentLEDController;
unsigned int StateComposer::ioPort;
unsigned int StateComposer::stripIndex;


// Initialization
bool StateComposer::initialize(bool log)
{
    logEnable = log;
    composerState = 'C';
    return true;
}

// Main composer function
void StateComposer::compose()
{
    tm* timeInfo;

    composerState = 'C';

    time(&sysTime);
    timeInfo=localtime(&sysTime);
    weekDay = timeInfo->tm_wday;
    unsigned int seconds = ( (timeInfo->tm_hour * 3600) + (timeInfo->tm_min * 60) + (timeInfo->tm_sec) );

    currentProfile = InternalState::get_current_profile();
    if (currentProfile == NULL) {
        // Error to log
        return;
    }

    // Will only loop over returned vector of zones (if none, skip)
    for (auto currentZone : currentProfile->get_zones()) {

        currentZoneSchedule = currentZone->get_schedule();
        if (currentZoneSchedule == NULL) {
            // Error to log
            continue;
        }

        currentZoneActiveState = currentZoneSchedule->get_active_state(seconds, weekDay);
        if (currentZoneActiveState == NULL) {
            // Error to log
            continue;
        }

        currentZoneLEDs = currentZone->get_leds();

        // Will only loop over returned vector of LEDs (if none, skip)
        for (auto currentLED : currentZoneLEDs) {

            currentLEDController = currentLED->get_controller();
            if (currentLEDController == NULL) {
                // Error to log
                continue;
            }

            ioPort = currentLEDController->get_io();
            if (ioPort <= 0) {
                // Error to log
                continue;
            }

            stripIndex = currentLED->get_strip_idx();
            if (stripIndex <= 0) {
                // Error to log
                continue;
            }

            // Gather and compute color data 
            // Call serial send
        }
    }
}