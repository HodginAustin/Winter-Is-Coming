#include <iostream>

#include "./includes/InternalState.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/API.hpp"


int main()
{
    // Internal state
    std::cout << "Initalizing Internal State...";
    if (InternalState::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }

    // Data parser
    std::cout << "Initalizing Data Parser...";
    if (DataParser::initialize()) { std::cout << "done" << std::endl; }
    else { std::cout << "failed" << std::endl; return 0; }


    // Test data
    Profile* p1 = new Profile();
    p1->set_id(DataParser::next_profile_id());
    p1->set_name("Test1");
    p1->set_description("Test Profile Number 1");

    Schedule* s1 = new Schedule(1);
    
    LED* l1 = new LED(1);

    Zone* z1 = new Zone();
    z1->set_id(1);
    z1->set_schedule(s1);
    z1->add_led(l1);

    p1->add_zone(z1);

    InternalState::add_profile(p1);


    // LED control system


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
    free(api);

    return 0;
}
