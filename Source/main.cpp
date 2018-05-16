#include <algorithm>
#include <iostream>

#include "./includes/InternalState.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/StateComposer.hpp"
#include "./includes/API.hpp"


void help() {

    std::cout << "\nUSAGE: ./plantergb [option] [&]\n\n"
              << "Options:\n"
              << "    -d      : Debug mode; log files created\n"
              << "    -p PORT : Specify PORT to run server on;\n"
              << "              (1024 - 65565) default is 9080\n\n"
              << std::flush;
}


char* get_cmd_opt(char ** begin, char ** end, const std::string& option)
{
    char** itr = std::find(begin, end-1, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }

    return *itr;
}


bool cmd_opt_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char** argv)
{
    // Arguments
    bool DEBUG = cmd_opt_exists(argv, argv+argc, "-d");
    unsigned int port_num = 9080;

    if (cmd_opt_exists(argv, argv+argc, "-p")) {
        port_num = atoi(get_cmd_opt(argv, argv+argc-1, "-p"));
    } 
    
    if (cmd_opt_exists(argv, argv+argc, "-h")) {
        help();
        return 0;
    }

    // Use LED simulator instead of normal serial data transmission
    bool USE_SIMULATOR = cmd_opt_exists(argv, argv+argc, "-sim");

    // Required LED intensity scalar to use all 60
    // LEDs at full white, off of the Arduino 5V rail
    // 5V rail can drive a maximum of 500mA
    // 60 * .2 * .033 = .396A (396mA)
    // Also makes the light bearable to look at...

    // We All Didn't Know Any Better Stupidity Inhibitor
    // TODO: Update once better power system is found
    float WADKABSI = 0.2f;
    if (cmd_opt_exists(argv, argv+argc, "-wadkabsi")) {
        WADKABSI = std::min((float)atof(get_cmd_opt(argv, argv+argc-1, "-wadkabsi")), 1.0f);
    }    

    // Bool success
    int boot = 0;

    // Internal state
    if (!InternalState::initialize()) { boot = 1; }

    // Data parser
    if (!DataParser::initialize(DEBUG)) { boot = 2; }

    // LED control system
    if (!StateComposer::initialize(DEBUG, USE_SIMULATOR, WADKABSI)) { boot = 3; }

    // API
    Port port(port_num);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);
    if (!api->initialize(threads, DEBUG)) { boot = 4; }
    api->start(port_num);

    // Cleanup
    StateComposer::clean_up();
    InternalState::clean_up();
    api->clean_up();
    free(api);

    return boot;
}
