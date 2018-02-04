#include "./includes/StateComposer.hpp"


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
    while(1) {

        composerState = 'C';

        tm* timeInfo;
        time(&sysTime);
        timeInfo=localtime(&sysTime);
        weekDay = timeInfo->tm_wday;
        // Further get time values for logging and scheduling

        currentProfile = InternalState::get_current_profile();
        if (currentProfile == NULL) {
            // Error to log
            continue;
        }

        // Will only loop over returned vector of zones (if none, skip)
        for (auto currentZone : currentProfile->get_zones()) {

            currentZoneSchedule = currentZone->get_schedule();
            if (currentZoneSchedule == NULL) {
                // Error to log
                continue;
            }

            currentZoneActiveState = currentZoneSchedule->get_active_state(time, weekDay);
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
}