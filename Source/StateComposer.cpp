#include <iostream>

#include "./includes/StateComposer.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/Settings.hpp"

// The I^2C serial bus device in the linux system
#define I2C_BUS "/dev/i2c-1"

// Required LED intensity scalar to use all 60 
// LEDs at full white, off of the Arduino 5V rail
// 5V rail can drive a maximum of 500mA
// 60 * .2 * .033 = .396A (396mA)
// Also makes the light bearable to look at...

// We All Didn't Know Any Better Stupidity Inhibitor
// TODO: Update once better power system is found
#define WADKABSI 0.2

// The amount of microseconds to wait for the 
// Arduino Nano to send it's ACK and catch up
// last updated to: 20000
#define WAIT 20000

// Set the maximum number of times to reattempt
// writing serial data in the serial_send function
// if errors occur
#define TRIES 2



// Threading
pthread_t StateComposer::composerThread;

// State
char StateComposer::composerState;

// I2C
int StateComposer::i2cFileStream;

// Composer variables
std::ofstream StateComposer::logFile;
unsigned int StateComposer::i2cAddressOffset;

// Timing
time_t StateComposer::sysTime;
char StateComposer::timeBuffer[30];
tm* StateComposer::timeInfo;
int StateComposer::weekDay;
unsigned int StateComposer::seconds;

// Internal State variables
Profile* StateComposer::currentProfile;
LEDState* StateComposer::currentZoneActiveState;

unsigned char StateComposer::red;
unsigned char StateComposer::green;
unsigned char StateComposer::blue;
int  StateComposer::intensity;
bool StateComposer::power;

std::vector<LED*> StateComposer::currentZoneLEDs;
Controller* StateComposer::currentLEDController;

unsigned char StateComposer::ioPort;
unsigned char StateComposer::stripIndex;


// Thread work
void* StateComposer::thr_compose_call(void*)
{
    // Begin composing
    StateComposer::set_composer_state('c');

    std::cout << "Started State Composer" << std::endl;
    while (StateComposer::get_composer_state() != 0) {          // Any non-off state
        if (StateComposer::get_composer_state() != 'p') {       // Pause state
            StateComposer::compose();

            // Enable pausing and shutdown
            if (StateComposer::get_composer_state() == 's') {   // LED Shutdown state
                StateComposer::led_shutdown();
                StateComposer::set_composer_state('c');
            }
            
            // Sleep for .25 seconds, give the core a break during no state
            usleep(250000);
        }
    }
    pthread_exit(NULL);
}


// Initialization
bool StateComposer::initialize(bool logEnable)
{
    std::cout << "Initializing State Composer... ";
    
    i2cAddressOffset = Settings::get_setting(DataParser::NANO_IO_OFFSET).int_value;

    // Logging
    time(&sysTime);
    timeInfo = localtime(&sysTime);
    strftime(timeBuffer, 30, "%F.composer.log", timeInfo);

    if (logEnable) {
        logFile.open(timeBuffer);
        strftime(timeBuffer, 30, "%c", timeInfo);
        logFile << "[" << timeBuffer << "] StateComposer transcript started\n" << std::flush;
    }

    // Open in non terminal, non blocking, read-write mode
    i2cFileStream = open(I2C_BUS, O_RDWR | O_NOCTTY | O_NDELAY);

	if (i2cFileStream == -1) {  // ERROR - CAN'T OPEN SERIAL PORT
        std::cout << "failed" << std::endl;
		logFile << "    ERROR: Unable to open I2C device: "
                << I2C_BUS
                << "! \n           Ensure it is not in use by another application."
                << std::endl;
        return false;
	}

    // Threading
    int thrStatus = pthread_create(&composerThread, NULL, StateComposer::thr_compose_call, NULL);

    if (thrStatus) {
        std::cout << "failed" << std::endl;
        logFile << "   ERROR: Unable to create thread! Exiting --- " << thrStatus << std::endl;
        return false;
    }
    else { std::cout << "spun and done" << std::endl; }

    return true;
}



// Send and receive serial over I2C w/ correct timings
bool StateComposer::serial_send(unsigned char io, unsigned char r, unsigned char g, unsigned char b, unsigned char idx)
{
	unsigned char s_buffer[4];
	unsigned char* p_s_buffer;
    unsigned char attempts = 0;
    
    // Get current time
    time(&sysTime);
    timeInfo=localtime(&sysTime);
    strftime(timeBuffer, 30, "%c", timeInfo);

    if (logFile.is_open()) {
        logFile << "[" << timeBuffer << "] "
                << "Attempting I2C send:\n"
                << "  Idx:" << (int)idx << "\n"
                << "  R:" << (int)r << "\n"
                << "  G:" << (int)g << "\n"
                << "  B:" << (int)b << "\n";
    }

	p_s_buffer = &s_buffer[0];  // Reset pointer to head of array

	*p_s_buffer++ = idx;
    *p_s_buffer++ = r;
    *p_s_buffer++ = g;
    *p_s_buffer++ = b;
	
	if (i2cFileStream != -1) {

        if (ioctl(i2cFileStream, I2C_SLAVE, (io + i2cAddressOffset))) {   // Set io control for the I2C file stream, as sending to I2C slave, at address
            logFile << "ERROR: Can't switch ioctl to I2C bus address: [ " << (io + i2cAddressOffset) << " ]" << std::endl;
            return true;    // Error state; return value not used currently
        }



		int count = write(i2cFileStream, &s_buffer[0], (p_s_buffer - &s_buffer[0]));    // Filestream, bytes to write, number of bytes to write

		while (count < 0) {                                                             // If error in transmission, keep trying

            logFile << "ERROR: I2C transmit failed! [ " << (io + i2cAddressOffset) << " ]" << std::endl;

            count = write(i2cFileStream, &s_buffer[0], (p_s_buffer - &s_buffer[0]));    // Rewrite the same values
            usleep(WAIT);                                                               // Before blasting serial data again, let Arduino catch up
            attempts++;
            
            if (attempts == TRIES) {                                                    // Give up after TRIES attempts
                return true;
            }
		}

        usleep(WAIT);                                                                   // Even if no error, let Arduino catch up
	}

	return false;
}



// Main composer function
void StateComposer::compose()
{
    float scalar = 0.0;

    // Get current time
    time(&sysTime);
    timeInfo=localtime(&sysTime);
    strftime(timeBuffer, 30, "%c", timeInfo);

    weekDay = timeInfo->tm_wday;
    seconds = ( (timeInfo->tm_hour * 3600) + (timeInfo->tm_min * 60) + (timeInfo->tm_sec) );
    
    currentProfile = InternalState::get_current_profile();
    if (currentProfile == NULL) {
        return;
    }

    // Will only loop over returned vector of zones (if none, skip)
    for (auto currentZone : currentProfile->get_zones()) {

        currentZoneActiveState = currentZone->get_active_state(seconds, weekDay);
        if (currentZoneActiveState == NULL) {
            continue;
        }

        // Gather and compute color data
        intensity = currentZoneActiveState->get_intensity();
        power = currentZoneActiveState->get_power();

        scalar = (float)( ((float)intensity / 100.0) * (float)power * WADKABSI );

        if (logFile.is_open()) {
            logFile << "[" << timeBuffer << "] Power Scalar for current zone: " << scalar << "\n";
        }

        red = ((unsigned char) ( ((float)currentZoneActiveState->get_r()) * scalar)); 
        green = ((unsigned char) ( ((float)currentZoneActiveState->get_g()) * scalar));
        blue = ((unsigned char) ( ((float)currentZoneActiveState->get_b()) * scalar));

        currentZoneLEDs = currentZone->get_leds();

        if (logFile.is_open()) {
            logFile << "[" << timeBuffer << "] Looping on zone '" << currentZone->get_name() << "' LEDs\n";
        }

        // Will only loop over returned vector of LEDs (if none, skip)
        for (auto currentLED : currentZoneLEDs) {
        
            // Get controller info
            currentLEDController = currentLED->get_controller();
            if (currentLEDController == NULL) {
                continue;
            }

            ioPort = (unsigned char)currentLEDController->get_io();
            if (ioPort <= 0) {
                continue;
            }

            // Get LED index
            stripIndex = (unsigned char)currentLED->get_strip_idx();
            if (stripIndex < 0) {
                continue;
            }

            serial_send(ioPort, red, green, blue, stripIndex);
            // END OF LEDS LOOP
        }

        // END OF ZONES LOOP
    }

    // END OF CURRENT STATE ORCHESTRATION
}


void StateComposer::led_shutdown()
{
    for (auto currentLED : InternalState::get_leds())
    {
        if (currentLED) {

            // Get controller info
            currentLEDController = currentLED->get_controller();
            if (currentLEDController == NULL) {
                continue;
            }

            // Controller IO (deviceID)
            ioPort = (unsigned char)currentLEDController->get_io();
            if (ioPort <= 0) {
                continue;
            }

            // Get LED index
            stripIndex = (unsigned char)currentLED->get_strip_idx();
            if (stripIndex < 0) {
                continue;
            }

            // Call serial send
            serial_send(ioPort, '\0', '\0', '\0', stripIndex);
        }
    }
}

// Cleanup
void StateComposer::clean_up() 
{
    // Stop composing
    set_composer_state(0);
    
    // Join thread
    std::cout << "Joining composer thread back to main... ";
    pthread_join(composerThread, NULL);
    std::cout << "done." << std::endl;

    if (logFile.is_open()) {
	    logFile << "[ SHUTDOWN RECEIVED ]" << std::endl << std::flush;
        logFile.close();
    }

    // Close I2C
    close(i2cFileStream);
}

// Accessors
char StateComposer::get_composer_state() 
{
    return composerState;
}

// Mutators
void StateComposer::set_composer_state(char state) 
{
    composerState = state;
}
