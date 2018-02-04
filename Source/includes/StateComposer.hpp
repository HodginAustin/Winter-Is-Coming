// Include Guard
#ifndef __STATECOMPOSER_H_INCLUDED__
#define __STATECOMPOSER_H_INCLUDED__
//=================================

#include <ctime>
#include <iostream>
#include <string>

#include "./InternalState.hpp"
#include "./Schedule.hpp"

class StateComposer
{
private:
    
    static struct color {
        // u-char : values from 0 - 255
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    // Log actions flag
    static bool logEnable;    
    // Composer's current state:
    //  C == computing on current internal state
    //  T == transmitting data to nano
    //  R == waiting for reply from nano
    static char composerState;
    // Timing objects
    static time_t sysTime;
    static char weekDay;
    // Communication objects
    static char acknowledge;
    static char retransmit;

    // Internal State working objects 
    // Structured by Internal State layers,
    // and looping through them
    static Profile* currentProfile;
        // Get vector of zones... for each, get:
            static Schedule* currentZoneSchedule;
            static LEDState* currentZoneActiveState;
            static std::vector<LED*> currentZoneLEDs;
                // For each LED, get: 
                static Controller* currentLEDController;
                    static unsigned int ioPort;
                    static unsigned int stripIndex;

    // Simple function that sends color state to a controller with target LED
    static bool send_serial(Controller*, color, unsigned int);
    // Log particular messages / actions
    static void log(const std::string &text);

public:
    // Initialization
    //  @Param 1 - Log actions flag
    //      True  : output actions to log
    //      False : don't ^
    static bool initialize(bool);

    // Start compositioning of the internal state to the hardware state
    static void compose();

    // Accessors
    static char get_composer_state();
    
};



#endif // __STATE_COMPOSER_INCLUDED__