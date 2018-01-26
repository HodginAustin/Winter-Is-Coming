#include <map>

#include <pistache/http.h>
#include <pistache/mime.h>

#include "./includes/json.hpp"

#include "./includes/API.hpp"
#include "./includes/InternalState.hpp"
#include "./includes/Schedule.hpp"

using namespace Pistache; // REST API
using json = nlohmann::json; // Json


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
    Routes::Get(router, "/", Routes::bind(&API::index, this));
    Routes::Get(router, "/restart", Routes::bind(&API::system_restart, this));

    // API Actions
    Routes::Get(router, "/shutdown", Routes::bind(&API::api_shutdown, this));

    // Profile routes
    Routes::Get(router, "/profiles", Routes::bind(&API::get_profiles, this));
    Routes::Get(router, "profiles/:id", Routes::bind(&API::get_profile, this));
    Routes::Get(router, "profiles/:id/zones", Routes::bind(&API::get_profile_zones, this));
    Routes::Get(router, "current_profile", Routes::bind(&API::get_current_profile, this));
    Routes::Post(router, "/profiles/add", Routes::bind(&API::post_profile, this));
    Routes::Post(router, "profiles/:id/zones/add", Routes::bind(&API::post_profile_zone, this));
    Routes::Delete(router, "profiles/:profile_id/zones/:zone_id/delete", Routes::bind(&API::delete_profile_zone, this));
    Routes::Delete(router, "profile/:id/delete", Routes::bind(&API::delete_profile, this));
    
    // Zone routes
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id", Routes::bind(&API::get_zone, this));
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id/schedule", Routes::bind(&API::get_zone_schedule, this));
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id/leds", Routes::bind(&API::get_zone_leds, this));
    Routes::Put(router, "profiles/:profile_id/zones/:zone_id/leds/add/", Routes::bind(&API::put_zone_led, this));
    Routes::Delete(router, "profiles/:profile_id/zones/:zone_id/leds/:led_id/delete",
                            Routes::bind(&API::delete_zone_led, this));

    // Schedule routes
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id/schedule/active_state", Routes::bind(&API::get_schedule_active_led_state, this));
    Routes::Put(router, "profiles/:profile_id/zones/:zone_id/schedule/day/:day_of_week/add/",
                            Routes::bind(&API::put_schedule_daily_state, this));
    Routes::Delete(router, "profiles/:profile_id/zones/:zone_id/schedule/day/:day_of_week/delete",
                            Routes::bind(&API::delete_schedule_daily_state, this));
    
    // LED routes
    Routes::Get(router, "leds", Routes::bind(&API::get_leds, this));
    Routes::Get(router, "leds/:id", Routes::bind(&API::get_led, this));
    Routes::Get(router, "leds/:id/controller", Routes::bind(&API::get_led_controller, this));
    Routes::Put(router, "leds/:id/controller/add", Routes::bind(&API::put_led_controller, this));
    Routes::Post(router, "leds/add", Routes::bind(&API::post_led, this));
    Routes::Delete(router, "leds/:id/delete", Routes::bind(&API::delete_led, this));
    Routes::Delete(router, "leds/:led_id/controller/delete", Routes::bind(&API::delete_led_controller, this));

    // Controller routes
    Routes::Get(router, "controllers", Routes::bind(&API::get_controllers, this));
    Routes::Get(router, "controllers/:id", Routes::bind(&API::get_controller, this));
    Routes::Post(router, "controllers/add", Routes::bind(&API::post_controller, this));
    Routes::Delete(router, "controllers/:id/delete", Routes::bind(&API::delete_controller, this));

    // LEDState routes
    Routes::Get(router, "led_states", Routes::bind(&API::get_led_states, this));
    Routes::Get(router, "led_states/:id", Routes::bind(&API::get_led_state, this));        
    Routes::Post(router, "led_states/add", Routes::bind(&API::post_led_state, this));
    Routes::Delete(router, "led_states/:id/delete", Routes::bind(&API::delete_led_state, this));

    // DailyState routes
    Routes::Get(router, "daily_states", Routes::bind(&API::get_daily_states, this));
    Routes::Get(router, "daily_states/:id", Routes::bind(&API::get_daily_state, this));
    Routes::Get(router, "daily_states/:id/led_states", Routes::bind(&API::get_daily_state_led_states, this));
    Routes::Put(router, "daily_states/:daily_state_id/led_states/add",
                        Routes::bind(&API::put_daily_state_led_state, this));
    Routes::Post(router, "daily_states/add", Routes::bind(&API::post_daily_state, this));
    Routes::Delete(router, "daily_states/:id/daily_states", Routes::bind(&API::delete_daily_state, this));
    Routes::Delete(router, "daily_states/:daily_state_id/led_states/:led_id/delete", Routes::bind(&API::delete_daily_state_led_state, this));

}


// JSON ENCODE / DECODE EXAMPLES
// Decode
//auto j = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
// Encode
// std::string s = j.dump();


// Logging
void API::log_req(REQUEST)
{
    std::cout << "Received request!" << std::endl;
}


// Controller endpoints (REST methods)

// System routes
void API::index(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Welcome to PlanteRGB!");
}
void API::system_restart(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Restarting");
}

// Actions
void API::api_shutdown(REQUEST, RESPONSE)
{
    response.send(Http::Code::Ok, "Shutting down");
    shutdown();
}


// Profile CRUD routes
void API::get_profiles(REQUEST, RESPONSE)
{
    log_req(request);

    // Json response MIME
    response.headers()
            .add<Http::Header::ContentType>(MIME(Application, Json));
    
    // Build JSON from profiles vector
    json j = json::array(); // Empty JSON array []
    std::vector<Profile*> profiles = InternalState::get_profiles();
    for (std::vector<Profile*>::iterator iter = profiles.begin(); iter < profiles.end(); iter++){
        j.push_back(*(*iter));
    }

    response.send(Http::Code::Ok, j.dump());
}
void API::get_profile(REQUEST, RESPONSE)
{
    auto id = request.param(":id").as<unsigned int>();
    Profile* profile = InternalState::get_profile(id);

    json j;
    Http::Code code;
    if (profile != 0){
        j = *profile;
        code = Http::Code::Ok;
    } else {
        code = Http::Code::Not_Found;
    }
    response.send(code, j.dump());
}
void API::get_profile_zones(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_current_profile(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_profile(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_profile_zone(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_profile_zone(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_profile(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// Zone routes
void API::get_zone(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_zone_schedule(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_zone_leds(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_zone_led(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_zone_led(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// Schedule routes
void API::get_schedule_active_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_schedule_daily_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_schedule_daily_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// LED routes
void API::get_leds(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_led_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_led(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// Controller routes
void API::get_controllers(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_controller(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// LEDState routes
void API::get_led_states(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// DailyState routes
void API::get_daily_states(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_daily_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_daily_state_led_states(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_daily_state_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_daily_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_daily_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_daily_state_led_state(REQUEST, RESPONSE)
{
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
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
