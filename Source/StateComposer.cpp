#include "./includes/StateComposer.hpp"

// The serial RxTx device in the linux system
// TODO: Verify the correct port is in use for the Pi model
#define SERIAL_DEV "/dev/serial0"

// Required LED intensity scalar to use all 60 
// LEDs at full white, off of the Arduino 5V rail
// 5V rail can drive a maximum of 500mA
// 60 * .2 * .033 = .396A (396mA)

// We All Didn't Know Any Better Stupidity Inhibitor
// TODO: Update once better power system is found
#define WADKABSI 0.2

// The amount of microseconds to wait for the 
// Arduino Nano to send it's ACK and catch up
// last updated to: 12000
#define WAIT_ACK 12000


// Required for static class members
bool StateComposer::composeEnable;

// UART
int StateComposer::uartFilestream;
struct termios StateComposer::options;

// Composer variables
bool StateComposer::logEnable;
std::ofstream StateComposer::logFile;
char StateComposer::composerState;

// Timing
time_t StateComposer::sysTime;
int StateComposer::weekDay;
unsigned int StateComposer::seconds;

// Internal State variables
Profile* StateComposer::currentProfile;
Schedule* StateComposer::currentZoneSchedule;
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



// Initialization
bool StateComposer::initialize(bool log)
{
    composeEnable = true;
    
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

    uartFilestream = open(SERIAL_DEV, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

	if (uartFilestream == -1) { // ERROR - CAN'T OPEN SERIAL PORT
		std::cerr << "ERROR: Unable to open UART on " << SERIAL_DEV << "! \nEnsure it is not in use by another application.\n" << std::endl;
        return false;
	}

    // CONFIGURE THE UART
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)

    tcgetattr(uartFilestream, &options);
	options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;		// Set baud rate, bits in-transmission, etc
	options.c_iflag = IGNPAR;                               // Ignore parity
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uartFilestream, TCIFLUSH);
	tcsetattr(uartFilestream, TCSANOW, &options);

    return true;
}



// Send and receive serial over uart w/ correct timings
bool StateComposer::serial_send(unsigned char io, unsigned char r, unsigned char g, unsigned char b, unsigned char idx)
{
    // Tx Bytes - Send LED data to proper controller
	unsigned char tx_buffer[5];
	unsigned char *p_tx_buffer;
    char timeBuffer[30];

    tm* timeInfo;
    time(&sysTime);
    timeInfo=localtime(&sysTime);

    if (logEnable) {
        strftime(timeBuffer, 30, "%c", timeInfo);
        logFile << "[" << timeBuffer << "] "
                        << "Attempting serial send:\n"
                        << "  IO Port:" << (int)io << "\n"
                        << "  Idx:" << (int)idx << "\n"
                        << "  R:" << (int)r << "\n"
                        << "  G:" << (int)g << "\n"
                        << "  B:" << (int)b << "\n";
    }

	p_tx_buffer = &tx_buffer[0]; // Reset pointer to head of array
    *p_tx_buffer++ = io;
	*p_tx_buffer++ = idx;
    *p_tx_buffer++ = r;
    *p_tx_buffer++ = g;
    *p_tx_buffer++ = b;
	
	if (uartFilestream != -1) {

		int count = write(uartFilestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));    // Filestream, bytes to write, number of bytes to write
		if (count < 0) {

			std::cerr << "   UART Tx error!\n" << std::endl;
            return true;
		}
        
        usleep(WAIT_ACK); // Let Arduino catch up
	}



    // Rx Bytes - Receive Acknowledge
    unsigned char rx_buffer[4];
    int rx_length = read(uartFilestream, (void*)rx_buffer, 3);		//Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0) {
        //An error occured (will occur if there are no bytes)
        logFile << "[" << timeBuffer << "] ERROR: Incorrect/no repsonse from Nano!\n";
    }
    else if (rx_length == 0) {
        //No data waiting
    }
    else {
        //Bytes received
        rx_buffer[rx_length] = '\0';
        logFile << "[" << timeBuffer << "] Received response: " << rx_buffer << std::endl;
    }

	return false;
}



// Main composer function
void StateComposer::compose()
{
    composerState = 'C';
    float scalar = 0.0;
    char timeBuffer[30];

    tm* timeInfo;
    time(&sysTime);
    timeInfo=localtime(&sysTime);

    weekDay = timeInfo->tm_wday;
    seconds = ( (timeInfo->tm_hour * 3600) + (timeInfo->tm_min * 60) + (timeInfo->tm_sec) );

    if (logEnable) {
        strftime(timeBuffer, 30, "%c", timeInfo);
        logFile << "[" << timeBuffer << "] Starting composition of internal state to hardware\n";
    }
    
    currentProfile = InternalState::get_current_profile();
    if (currentProfile == NULL) {
        if (logEnable) {
            logFile << "[" << timeBuffer << "] No profiles to loop on. Exiting composer\n";
        }
        return;
    }

    if (logEnable) {
        logFile << "[" << timeBuffer << "] Looping on active profile zones\n";
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

        if (logEnable)
            logFile << "[" << timeBuffer << "] Power Scalar for current zone: " << scalar << "\n";

        red = ((unsigned char) ( ((float)currentZoneActiveState->get_r()) * scalar)); 
        green = ((unsigned char) ( ((float)currentZoneActiveState->get_g()) * scalar));
        blue = ((unsigned char) ( ((float)currentZoneActiveState->get_b()) * scalar));

        currentZoneLEDs = currentZone->get_leds();

        if (logEnable) {
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
            stripIndex = currentLED->get_strip_idx();
            if (stripIndex < 0) {
                continue;
            }

            // Call serial send
            composerState = 'S';

            if (serial_send(ioPort, red, green, blue, stripIndex)) {
                logFile << "[" << timeBuffer << "] " << "Error transmitting serial!\n";
            }
            composerState = 'C';
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
        stripIndex = currentLED->get_strip_idx();
        if (stripIndex < 0) {
            continue;
        }

        // Call serial send
        if (serial_send(ioPort, '\0', '\0', '\0', stripIndex)) {
            logFile << "[" << timeBuffer << "] " << "Error transmitting serial for led_shutdown!\n";
        }
    }
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