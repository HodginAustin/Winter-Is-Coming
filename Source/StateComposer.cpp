#include "./includes/StateComposer.hpp"


// Required for static class members

// UART
int StateComposer::uartFilestream;
struct termios StateComposer::options;

// Composer variables
bool StateComposer::logEnable;
char StateComposer::composerState;

// Internal State variables
time_t StateComposer::sysTime;
int StateComposer::weekDay;
Profile* StateComposer::currentProfile;
Schedule* StateComposer::currentZoneSchedule;
LEDState* StateComposer::currentZoneActiveState;
char StateComposer::red;
char StateComposer::green;
char StateComposer::blue;
int  StateComposer::intensity;
bool StateComposer::power;
std::vector<LED*> StateComposer::currentZoneLEDs;
Controller* StateComposer::currentLEDController;
unsigned int StateComposer::ioPort;
unsigned int StateComposer::stripIndex;


// Initialization
bool StateComposer::initialize(bool log)
{
    logEnable = log;
    composerState = 'C';

    // TODO: Need to check dev file for the Pi 0 W
    uartFilestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uartFilestream == -1)
	{
		// ERROR - CAN'T OPEN SERIAL PORT
		printf("ERROR: Unable to open UART!\nEnsure it is not in use by another application.\n");
        return false;
	}

    //CONFIGURE THE UART
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)

    tcgetattr(uartFilestream, &options);
	options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;		// Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uartFilestream, TCIFLUSH);
	tcsetattr(uartFilestream, TCSANOW, &options);

    return true;
}

bool StateComposer::r_t_serial(Controller* ctrlr, char r, char g, char b, unsigned int idx)
{

	return true;
}



// Main composer function
void StateComposer::compose()
{
    tm* timeInfo;

    composerState = 'C';
    float scalar = 0.0;

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
        

            // Get controller info
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

            // Get LED index
            stripIndex = currentLED->get_strip_idx();
            if (stripIndex <= 0) {
                // Error to log
                continue;
            }

            // Gather and compute color data
            intensity = currentZoneActiveState->get_intensity();
            power = currentZoneActiveState->get_power();

            scalar = (float)(((float)intensity / 100.0) * (float)power);

            red = (int ( ((float)currentZoneActiveState->get_r()) * scalar)); 
            green = (int ( ((float)currentZoneActiveState->get_g()) * scalar));
            blue = (int ( ((float)currentZoneActiveState->get_b()) * scalar));



            // Call serial send
            composerState = 'S';
            if (r_t_serial(currentLEDController, red, green, blue, stripIndex)) {
                // Error to log
            } 
            composerState = 'C';
            // END OF LEDS LOOP
        }

        // END OF ZONES LOOP
    }

}
