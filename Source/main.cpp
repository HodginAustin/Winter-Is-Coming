#include <iostream>

#include "./includes/InternalState.hpp"
#include "./includes/API.hpp"


int main()
{
    // Internal state
    std::cout << "Initalizing Internal State...";
    if (InternalState::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; }


    // Test data
    Profile* p = new Profile(1);
    p->set_name("CoolProfile");
    InternalState::add_profile(p);


    // Data parser

    // LED control system

    // API (needs to be the last thing in this function)
    Port port(9080);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);

    std::cout << "Initalizing API...";
    if (api->initialize(threads)) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; }

    // Start listening
    std::cout << "Started listening on port " << port << std::endl;
    api->start();

    // Cleanup
    free(api);

    return 0;
}
