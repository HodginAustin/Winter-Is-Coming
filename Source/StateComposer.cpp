#include <iostream>
#include <cstdlib>

#include "./includes/StateComposer.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/Settings.hpp"
#include "./includes/LEDStateSmall.hpp"
#include "./includes/LEDState.hpp"

// The I^2C serial bus device in the linux system
#define I2C_BUS "/dev/i2c-1"

// The amount of microseconds to wait for the
// Arduino Nano to send it's ACK and catch up
// last updated to: 18000 (for with serial write on nano)
#define WAIT 18000

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


// Parameters
bool StateComposer::USE_SIMULATOR;
float StateComposer::WADKABSI;

// Composer variables
std::ofstream StateComposer::logFile;
unsigned int StateComposer::i2cAddressOffset;

std::unordered_map<unsigned int, struct bareLEDState*>::const_iterator prevLEDStateIter;
std::unordered_map<unsigned int, struct bareLEDState*> previousLEDStates;

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
bool StateComposer::initialize(bool logEnable, bool useSimulator, float wadkabsi)
{
    std::cout << "Initializing State Composer... ";
    
    i2cAddressOffset = Settings::get_setting(DataParser::NANO_IO_OFFSET).int_value;

    // Parameters
    USE_SIMULATOR = useSimulator;
    WADKABSI = wadkabsi;

    // Logging
    time(&sysTime);
    timeInfo = localtime(&sysTime);
    strftime(timeBuffer, 30, "%F.composer.log", timeInfo);

    if (logEnable) {
        logFile.open(timeBuffer);
        strftime(timeBuffer, 30, "%c", timeInfo);
        logFile << "[" << timeBuffer << "] StateComposer transcript started\n" << std::flush;
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
	//unsigned char* p_s_buffer;
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
                << "  B:" << (int)b << "\n"
                << std::flush;
    }

	//p_s_buffer = &s_buffer[0];  // Reset pointer to head of array

	//*p_s_buffer++ = idx;
    //*p_s_buffer++ = r;
    //*p_s_buffer++ = g;
    //*p_s_buffer++ = b;

    s_buffer[0] = idx;
    s_buffer[1] = r;
    s_buffer[2] = g;
    s_buffer[3] = b;

    // Open in non terminal, non blocking, read-write mode
    i2cFileStream = open(I2C_BUS, O_RDWR | O_NOCTTY | O_NDELAY);

	if (i2cFileStream != -1) {

        if (ioctl(i2cFileStream, I2C_SLAVE, (io + i2cAddressOffset))) {   // Set io control for the I2C file stream, as sending to I2C slave, at address
            logFile << "[" << timeBuffer << "] ERROR: Can't switch ioctl to I2C bus address: [ " << (io + i2cAddressOffset) << " ] !\n" << std::flush;
            close(i2cFileStream);                                                       // Close file stream, move on
            return true;                                                                // Error state; return value not used currently
        }

		//int count = write(i2cFileStream, &s_buffer[0], (p_s_buffer - &s_buffer[0]));    // Filestream, bytes to write, number of bytes to write
        int count = write(i2cFileStream, &s_buffer, 4);

		while (count != 4) {                                                            // If error in transmission, keep trying

            logFile << "[" << timeBuffer << "] ERROR: I2C transmit failed! [ " << (io + i2cAddressOffset) << " ] Sent " << count << "\n" << std::flush;

            //count = write(i2cFileStream, &s_buffer[0], (p_s_buffer - &s_buffer[0]));    // Rewrite the same values
            count = write(i2cFileStream, &s_buffer, 4);
            usleep(WAIT);                                                               // Before blasting serial data again, let Arduino catch up
            attempts++;

            if (attempts == TRIES) {                                                    // Give up after TRIES attempts
                close(i2cFileStream);                                                   // Close file stream, move on
                return true;
            }
		}

        usleep(WAIT);                                                                   // Even if no error, let Arduino catch up
	}

    else {
        logFile << "[" << timeBuffer << "] ERROR: Couldn't open I2C filestream! Ensure it is not in use\n" << std::flush;
        close(i2cFileStream);
        return true;
    }

    close(i2cFileStream);                                                               // Close file stream when done

	return false;
}


/* Sends update request via cURL for simulator */
/* this is used for testing when hardware is not available.*/
/* Set the Use Simulator parameter to True to use this */
bool StateComposer::serial_send_test(unsigned char io, unsigned char r, unsigned char g, unsigned char b, unsigned char idx)
{
    float m = (1.0f / WADKABSI);
    int offset_r = (int)((float)((int)r) * m);
    int offset_g = (int)((float)((int)g) * m);
    int offset_b = (int)((float)((int)b) * m);
    char curl[256];
    sprintf(curl, "curl --silent -X POST http://localhost:8080/simulator/update/%d/%d/%d/%d/%d > /dev/null", (int)io, (int)offset_r, (int)offset_g, (int)offset_b, (int)idx);
    system(curl); /* sends system command to run in the terminal */
    usleep(WAIT); /* to make things consistent with normal operations we still wait */
	return false;
}


// Dump previous state map, causes refresh
void StateComposer::refresh_state()
{
    previousLEDStates.clear();
}


// Main composer function
void StateComposer::compose()
{
    float scalar = 0.0;

    // Get current timecount = write(i2cFileStream, &s_buffer, 4);
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

        // Check previous LED state for this zone
        // continue on to next zone if it is the same
        prevLEDStateIter = previousLEDStates.find(currentZone->get_id());
        if (prevLEDStateIter != previousLEDStates.end()) {
            if ( LEDState::equals(prevLEDStateIter->second, currentZoneActiveState) ) {
                continue;
            }
            else {
                if (logFile.is_open()) 
                    logFile << "Zone state has changed, updating...\n" << std::flush;
                delete previousLEDStates[currentZone->get_id()];
            }
        }

        struct bareLEDState* temp = new bareLEDState();
        temp->r = currentZoneActiveState->get_r();
        temp->g = currentZoneActiveState->get_g(); 
        temp->b = currentZoneActiveState->get_b(); 
        temp->intensity = currentZoneActiveState->get_intensity();
        temp->power = currentZoneActiveState->get_power();
       
        // Save previous LED state
        // State not equal, saving it
        previousLEDStates[currentZone->get_id()] = temp;
       
        // Gather and compute color data
        intensity = currentZoneActiveState->get_intensity();
        power = currentZoneActiveState->get_power();

        scalar = (float)( ((float)intensity / 100.0) * (float)power * WADKABSI );

        if (logFile.is_open()) {
            logFile << "[" << timeBuffer << "] Power Scalar for current zone: " << scalar << "\n" << std::flush;
        }

        red = ((unsigned char) ( ((float)currentZoneActiveState->get_r()) * scalar));
        green = ((unsigned char) ( ((float)currentZoneActiveState->get_g()) * scalar));
        blue = ((unsigned char) ( ((float)currentZoneActiveState->get_b()) * scalar));

        currentZoneLEDs = currentZone->get_leds();

        if (logFile.is_open()) {
            logFile << "[" << timeBuffer << "] Looping on zone '" << currentZone->get_name() << "' LEDs\n" << std::flush;
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
            
            // Use simulator or send over I2C
            if (USE_SIMULATOR) {
                serial_send_test(ioPort, red, green, blue, stripIndex);
            } else {
                serial_send(ioPort, red, green, blue, stripIndex);
            }
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
            if (USE_SIMULATOR) {
                serial_send_test(ioPort, '\0', '\0', '\0', stripIndex);
            } else {
                serial_send(ioPort, '\0', '\0', '\0', stripIndex);
            }
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
    if ( (state == 'c') || (state == 'p') || (state == 's') || (state == 0) )   // Check if valid state
        composerState = state;
}
