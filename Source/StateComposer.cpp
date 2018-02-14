#include "./includes/StateComposer.hpp"

// The serial RxTx device in the linux system
#define SERIAL_DEV "/dev/serial0"


// Required for static class members

// UART
int StateComposer::uartFilestream;
struct termios StateComposer::options;

// Composer variables
bool StateComposer::logEnable;
std::ofstream StateComposer::logFile;
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

    tm* timeInfo;
    time(&sysTime);
    timeInfo = localtime(&sysTime);
    char timeBuffer[30];
    strftime(timeBuffer, 30, "%c", timeInfo);

    if (logEnable) {
        logFile.open("composer.log");
        logFile << "[" << timeBuffer << "] StateComposer transcript started\n";
    }

    // TODO: Need to check dev file for the Pi 0 W
    uartFilestream = open(SERIAL_DEV, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

	if (uartFilestream == -1) { // ERROR - CAN'T OPEN SERIAL PORT
		std::cerr << "ERROR: Unable to open UART on " << SERIAL_DEV << "! \nEnsure it is not in use by another application.\n" << std::endl;
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

bool StateComposer::serial_send(Controller* ctrlr, char r, char g, char b, unsigned int idx)
{
    //----- TX BYTES -----
	unsigned char tx_buffer[4];
	unsigned char *p_tx_buffer;
	
	p_tx_buffer = &tx_buffer[0];
	*p_tx_buffer++ = idx;
    *p_tx_buffer++ = r;
    *p_tx_buffer++ = g;
    *p_tx_buffer++ = b;
	
	if (uartFilestream != -1)
	{
		int count = write(uartFilestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));    // Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			std::cerr << "   UART Tx error!\n" << std::endl;
            return true;
            
		}
	}

	return false;
}



// Main composer function
void StateComposer::compose()
{
    composerState = 'C';
    float scalar = 0.0;

    tm* timeInfo;
    time(&sysTime);
    timeInfo=localtime(&sysTime);

    weekDay = timeInfo->tm_wday;
    unsigned int seconds = ( (timeInfo->tm_hour * 3600) + (timeInfo->tm_min * 60) + (timeInfo->tm_sec) );

    char timeBuffer[30];

    if (logEnable)
        strftime(timeBuffer, 30, "%c", timeInfo);

    currentProfile = InternalState::get_current_profile();
    if (currentProfile == NULL) {
        if (logEnable) {
            logFile << "[" << timeBuffer << "] No profiles to loop on. Exiting composer\n";
        }
        return;
    }

    // Will only loop over returned vector of zones (if none, skip)
    for (auto currentZone : currentProfile->get_zones()) {

        currentZoneActiveState = currentZone->get_active_state(seconds, weekDay);
        if (currentZoneActiveState == NULL) {
            continue;
        }

        currentZoneLEDs = currentZone->get_leds();

        // Will only loop over returned vector of LEDs (if none, skip)
        for (auto currentLED : currentZoneLEDs) {
        

            // Get controller info
            currentLEDController = currentLED->get_controller();
            if (currentLEDController == NULL) {
                continue;
            }

            ioPort = currentLEDController->get_io();
            if (ioPort <= 0) {
                continue;
            }

            // Get LED index
            stripIndex = currentLED->get_strip_idx();
            if (stripIndex <= 0) {
                continue;
            }

            // Gather and compute color data
            intensity = currentZoneActiveState->get_intensity();
            power = currentZoneActiveState->get_power();

            scalar = (float)(((float)intensity / 100.0) * (float)power);

            if (logEnable)
                logFile << "[" << timeBuffer << "] Power Scalar: " << scalar << "\n";

            red = (int ( ((float)currentZoneActiveState->get_r()) * scalar)); 
            green = (int ( ((float)currentZoneActiveState->get_g()) * scalar));
            blue = (int ( ((float)currentZoneActiveState->get_b()) * scalar));

            if (logEnable) {
                logFile << "[" << timeBuffer << "] "
                        << "Attempting serial send:\n"
                        << "  IO Port:" << ioPort << "\n"
                        << "  R:" << red << "\n"
                        << "  G:" << green << "\n"
                        << "  B:" << blue << "\n"
                        << "  Idx:" << stripIndex << "\n";
            }

            // Call serial send
            composerState = 'S';

            if (serial_send(currentLEDController, red, green, blue, stripIndex)) {
                logFile << "[" << timeBuffer << "] " << "Error transmitting serial\n";
            } 
            composerState = 'C';
            // END OF LEDS LOOP
        }

        // END OF ZONES LOOP
    }

    // END OF CURRENT STATE ORCHESTRATION
}



void StateComposer::clean_up() 
{
    if (logEnable)
        logFile.close();

    close(uartFilestream);
}



char StateComposer::get_composer_state() 
{
    return composerState;
}