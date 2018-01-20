// Include Guard
#ifndef __API_H_INCLUDED__
#define __API_H_INCLUDED__
//=================================

#include <pistache/endpoint.h>
#include <pistache/router.h>


#define REQUEST_RESPONSE const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response


using namespace Pistache;


class API {
public:
    API(Address addr);

    bool initialize(size_t thr);
    void start();
    void shutdown();

private:
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    volatile bool api_power_state = true;

    void setup_routes();


    // Controller functions (REST methods)
    // System routes
    void api_shutdown(REQUEST_RESPONSE);

    void get_profile(REQUEST_RESPONSE);
};

#endif //__API_H_INCLUDED__
