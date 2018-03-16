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

    // Internal state
    if (!InternalState::initialize()) { return 1; }

    // Data parser
    if (!DataParser::initialize(DEBUG)) { return 1; }

    // LED control system
    if (!StateComposer::initialize(DEBUG)) { return 1; }

    // API
    Port port(port_num);
    Address addr(Ipv4::any(), port);
    int threads = 2;
    API* api = new API(addr);

    if (!api->initialize(threads, DEBUG)) { return 1; }
    api->start(port_num);

    // Cleanup
    StateComposer::clean_up();
    InternalState::clean_up();
    DataParser::clean_up();
    api->clean_up();
    free(api);

    return 0;
}
