#include "./includes/InternalState.hpp"
#include "./includes/DataParser.hpp"
#include "./includes/StateComposer.hpp"
#include "./includes/API.hpp"

// State Composer Logging
#define DEBUG true

int main(int argc, char* argv[])
{
    // Port number argument
    unsigned int port_num = (argc > 1 ? atoi(argv[1]) : 9080);

    // Bool success
    bool boot = true;

    // Internal state
    boot &= InternalState::initialize();
    if (!boot) { return 1; }

    // Data parser
    boot &= DataParser::initialize(DEBUG);
    if (!boot) { return 1; }

    // LED control system
    boot &= StateComposer::initialize(DEBUG);
    if (!boot) { return 1; }

    // API
    Port port(port_num);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);

    boot &= api->initialize(threads, DEBUG);
    if (!boot) { return 1; }
    api->start(port_num);

    // Cleanup
    StateComposer::clean_up();
    InternalState::clean_up();
    DataParser::clean_up();
    api->clean_up();
    free(api);

    return !boot;
}
