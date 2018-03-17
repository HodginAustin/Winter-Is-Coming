#include <algorithm>

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

    // Bool success
    bool boot = true;

    // Internal state
    boot &= InternalState::initialize();
    if (!boot) { return 1; }

    // Data parser
    boot &= DataParser::initialize(DEBUG);
    if (!boot) { return 2; }

    // LED control system
    boot &= StateComposer::initialize(DEBUG);
    if (!boot) { return 3; }

    // API
    Port port(port_num);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);
    boot &= api->initialize(threads, DEBUG);
    if (!boot) { return 4; }
    api->start(port_num);

    // Cleanup
    StateComposer::clean_up();
    InternalState::clean_up();
    api->clean_up();
    free(api);

    return !boot;
}
