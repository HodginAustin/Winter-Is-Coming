#include <iostream>
#include <pthread.h>

#include "./includes/InternalState.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/StateComposer.hpp"
#include "./includes/API.hpp"

// State Composer Logging
#define SC_LOG true


void* thr_compose_call(void*)
{
    std::cout << "Starting State Composer...";
    StateComposer::compose(SC_LOG);
    pthread_exit(NULL);
}


int main()
{

    int thrStatus = -57;

    // Internal state
    std::cout << "Initalizing Internal State...";
    if (InternalState::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    // Data parser
    std::cout << "Initalizing Data Parser...";
    if (DataParser::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }


    // LED control system
    std::cout << "Initializing State Composer...";
    if (StateComposer::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    std::cout << "Spinning thread for State Composer...";
    pthread_t composerThread;
    thrStatus = pthread_create(&composerThread, NULL, thr_compose_call, NULL);

    if (thrStatus) {
        std::cerr << "ERROR: Unable to create thread! Exiting --- " << thrStatus << std::endl;
        exit(-1);
    }

    // API (needs to be the last thing in this function)
    Port port(9080);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);

    std::cout << "Initalizing API...";
    if (api->initialize(threads)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    // Start listening
    std::cout << "Started listening on port " << port << std::endl;
    api->start();

    // Cleanup
    pthread_join(composerThread, NULL);
    free(api);


    return 0;
}
