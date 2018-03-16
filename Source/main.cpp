#include <iostream>
#include <pthread.h>
#include <algorithm>

#include "./includes/InternalState.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/StateComposer.hpp"
#include "./includes/API.hpp"


bool cmd_opt_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


void* thr_compose_call(void*)
{
    std::cout << "Starting State Composer..." << std::endl;
    while (StateComposer::composeEnable) {
        StateComposer::compose();
        usleep(250000);  // Sleep for .25 seconds, give the core a break during no state
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    bool log = false;

    // Port number argument
    unsigned int port_num = (argc > 1 ? atoi(argv[1]) : 9080);

    bool logEnable = cmd_opt_exists(argv, argv+argc, "-d");
    
    // Internal state
    std::cout << "Initalizing Internal State... ";
    if (InternalState::initialize(logEnable)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }


    // Data parser
    std::cout << "Initalizing Data Parser... ";
    if (DataParser::initialize(logEnable)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }


    // LED control system
    std::cout << "Initializing State Composer... ";
    if (StateComposer::initialize(logEnable)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    std::cout << "Spinning thread for State Composer... ";
    pthread_t composerThread;
    int thrStatus = pthread_create(&composerThread, NULL, thr_compose_call, NULL);

    if (thrStatus) {
        std::cerr << "   ERROR: Unable to create thread! Exiting --- " << thrStatus << std::endl;
        exit(-1);
    }
    else { std::cout << " spun" << std::endl; }


    // API (needs to be the last thing in this function)
    Port port(port_num);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);

    std::cout << "Initalizing API... ";
    if (api->initialize(threads, logEnable)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    // Start listening
    std::cout << "Started listening on port " << port << std::endl;
    api->start();


    // Cleanup
    StateComposer::composeEnable = false;
    StateComposer::clean_up();
    std::cout << "Joining composer thread back to main... ";
    pthread_join(composerThread, NULL);
    std::cout << "Joined!\nFreeing API... " << std::endl;
    free(api);
    std::cout << "done\n\nExiting..." << std::endl;

    return 0;
}
