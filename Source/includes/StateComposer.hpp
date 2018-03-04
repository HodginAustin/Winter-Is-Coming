// Include Guard
#ifndef __STATECOMPOSER_H_INCLUDED__
#define __STATECOMPOSER_H_INCLUDED__
//=================================

#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

#include <unistd.h>     //Used for UART
#include <fcntl.h>		//Used for UART
#include <termios.h>	//Used for UART

#include "./Controller.hpp"
#include "./InternalState.hpp"
#include "./LED.hpp"
#include "./LEDState.hpp"
#include "./Profile.hpp"
#include "./Schedule.hpp"
#include "./Zone.hpp"

class StateComposer
{
private:

    // RGB color values
    static unsigned char red;
    static unsigned char green;
    static unsigned char blue;
    static int intensity;
    static bool power;
    // UART
    static int uartFilestream;
    static struct termios options;
    // Log actions
    static std::ofstream logFile;
    static bool logEnable;    
    // Composer's current state:
    //  C == computing on current internal state
    //  S == Communicating Serial 
    static char composerState;
    // Timing objects
    static time_t sysTime;
    static int weekDay;
    static unsigned int seconds;
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
                    static unsigned char ioPort;
                    static unsigned char stripIndex;

    // Sends color state to a target controller with target LED
    static bool serial_send(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);

public:
    static bool composeEnable;

    // Initialization
    //  @Param 1 - Log actions flag
    //      True  : output actions to log
    //      False : don't ^
    static bool initialize(bool);

    // Start compositioning of the internal state to the hardware state
    static void compose();

    // Turn LEDs off
    static void led_shutdown();

    // Clean up
    static void clean_up();

    // Accessors
    static char get_composer_state();
    
};



#endif // __STATE_COMPOSER_INCLUDED__
