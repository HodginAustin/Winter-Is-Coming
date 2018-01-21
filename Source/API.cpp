#include <map>

#include <pistache/http.h>

#include "./includes/API.hpp"
#include "./includes/InternalState.hpp"

using namespace Pistache;


API::API(Address addr)
    : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
{ }

bool API::initialize(size_t thr = 2)
{
    auto opts = Http::Endpoint::options()
                .threads(thr)
                .flags(Tcp::Options::InstallSignalHandler);

    httpEndpoint->init(opts);
    setup_routes();
    return true;
}

void API::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serveThreaded();

    while(api_power_state){ sleep(1); }
    
    std::cout << "Shutting down" << std::endl;
    httpEndpoint->shutdown();
}

void API::shutdown()
{
    api_power_state = false;
}


void API::setup_routes()
{
    using namespace Rest;
    // System routes
    Routes::Get(router, "/shutdown", Routes::bind(&API::api_shutdown, this));

    // Profile routes
    Routes::Get(router, "/profiles", Routes::bind(&API::get_profiles, this));
    Routes::Get(router, "/profile/:id", Routes::bind(&API::get_profile, this));




    //Routes::Post(router, "/record/:name/:value?", Routes::bind(&StatsEndpoint::doRecordMetric, this));
    //Routes::Get(router, "/value/:name", Routes::bind(&StatsEndpoint::doGetMetric, this));
    //Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
    //Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));
}


// Controller endpoints (REST methods)
// System routes
void API::api_shutdown(REQUEST_RESPONSE)
{
    response.send(Http::Code::Ok, "Shutting down");
    shutdown();
}

// Profile routes
void API::get_profiles(REQUEST_RESPONSE)
{

}
void API::get_profile(REQUEST_RESPONSE)
{
    auto id = request.param(":id").as<unsigned int>();
    
    Profile* profile = InternalState::get_profile(id);

    if (profile != 0){
        response.send(Http::Code::Ok, profile->get_name());
    } else {
        response.send(Http::Code::Not_Found, "Profile not found with id " + std::to_string(id));
    }
}




// Example get and post
//void doRecordMetric(const Rest::Request& request, Http::ResponseWriter response) {
//    auto name = request.param(":name").as<std::string>();

//    Guard guard(metricsLock);
//    auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric& metric) {
//        return metric.name() == name;
//    });
//
//    int val = 1;
//    if (request.hasParam(":value")) {
//        auto value = request.param(":value");
//        val = value.as<int>();
//    }

//    if (it == std::end(metrics)) {
//        metrics.push_back(Metric(std::move(name), val));
//        response.send(Http::Code::Created, std::to_string(val));
//    }
//    else {
//        auto &metric = *it;
//        metric.incr(val);
//        response.send(Http::Code::Ok, std::to_string(metric.value()));
//    }

//}

//void doGetMetric(const Rest::Request& request, Http::ResponseWriter response) {
//    auto name = request.param(":name").as<std::string>();

//    Guard guard(metricsLock);
//    auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric& metric) {
//        return metric.name() == name;
//    });
//
//    if (it == std::end(metrics)) {
//        response.send(Http::Code::Not_Found, "Metric does not exist");
//    } else {
//        const auto& metric = *it;
//        response.send(Http::Code::Ok, std::to_string(metric.value()));
//    }

//}
