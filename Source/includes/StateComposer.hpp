// Include Guard
#ifndef __STATECOMPOSER_H_INCLUDED__
#define __STATECOMPOSER_H_INCLUDED__
//=================================

#include <ctime>
#include <string>
#include <pthread.h>
#include <fstream>

#include <fcntl.h>          // Used for I2C
#include <linux/i2c-dev.h>  // Used for I2C
#include <sys/ioctl.h>      // Used for I2C
#include <unistd.h>         // Used for I2C

#include "./Controller.hpp"
#include "./InternalState.hpp"
#include "./LED.hpp"
#include "./LEDState.hpp"
#include "./Profile.hpp"
#include "./Zone.hpp"

class StateComposer
{
private:
    // Threading
    static pthread_t composerThread;

    // RGB color values
    static unsigned char red;
    static unsigned char green;
    static unsigned char blue;
    static int intensity;
    static bool power;

    // I2C 
    static int i2cFileStream;               // File descriptor for serial stream
    static unsigned int i2cAddressOffset;   // I2C bus addresses start at 0x03, so add this offset 
                                            // to the fetched 'io' controller member to reach the correct address

    // Logging
    static std::ofstream logFile;

    // Timing objects
    static time_t sysTime;
    static char timeBuffer[30];
    static tm* timeInfo;
    static int weekDay;
    static unsigned int seconds;

    // Parameters
    static bool USE_SIMULATOR;
    static float WADKABSI;

    // Internal State working objects 
    // Structured by Internal State layers,
    // and looping through them
    static Profile* currentProfile;
        // Get vector of zones... for each, get:
            static LEDState* currentZoneActiveState;
            static std::vector<LED*> currentZoneLEDs;
                // For each LED, get: 
                static Controller* currentLEDController;
                    static unsigned char ioPort;
                    static unsigned char stripIndex;

    // Composer's current state:
    // 0(NULL) - off, non functioning
    // 'c' - on, compose loop
    // 'p' - pause, no activity
    // 's' - pause, led_shutdown
    static char composerState;

    // Sends color state to a target controller with target LED
    static bool serial_send(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
    static bool serial_send_test(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);


    // Turn LEDs off (DO NOT CALL)
    static void led_shutdown();
public:
    // Thread work
    static void* thr_compose_call(void*);

    // Initialization
    //  @Param 1 - Log actions flag
    //      True  : output actions to log
    //      False : don't ^
    //  @Param 2 - Use LED simulator flag
    //  @Param 3 - LED brightness ceiling, stupidity inhibitor
    static bool initialize(bool logEnable, bool useSimulator, float wadkabsi);

    // Dump cache and refresh states
    static void refresh_state();

    // Start compositioning of the internal state to the hardware state
    static void compose();

    // Clean up
    static void clean_up();

    // Accessors
    static char get_composer_state();

    // Mutators
    static void set_composer_state(char);
    
};



#endif // __STATE_COMPOSER_INCLUDED__
