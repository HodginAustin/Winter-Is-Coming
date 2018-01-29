#include <map>
#include <ctime>

#include <pistache/http.h>
#include <pistache/mime.h>

#include "./includes/json.hpp"

#include "./includes/API.hpp"
#include "./includes/InternalState.hpp"
#include "./includes/Schedule.hpp"
#include "./includes/DataParser.hpp"

using namespace Pistache; // REST API
using json = nlohmann::json; // Json

// Required for static class members
std::shared_ptr<Http::Endpoint> httpEndpoint;
Rest::Router router;


API::API(Address addr) : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

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
    Routes::Get(router, "profiles", Routes::bind(&API::get_profiles, this));
    Routes::Get(router, "profiles/:id", Routes::bind(&API::get_profile, this));
    Routes::Get(router, "profiles/:id/zones", Routes::bind(&API::get_profile_zones, this));
    Routes::Get(router, "current_profile", Routes::bind(&API::get_current_profile, this));
    Routes::Post(router, "current_profile/:id", Routes::bind(&API::post_current_profile, this));
    Routes::Post(router, "profiles/add", Routes::bind(&API::post_profile, this));
    Routes::Post(router, "profiles/:id/zones/add", Routes::bind(&API::post_profile_zone, this));
    Routes::Patch(router, "profiles/:id/edit", Routes::bind(&API::patch_profile, this));
    Routes::Delete(router, "profiles/:profile_id/zones/:zone_id/delete", Routes::bind(&API::delete_profile_zone, this));
    Routes::Delete(router, "profiles/:id/delete", Routes::bind(&API::delete_profile, this));
    
    // Zone routes
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id", Routes::bind(&API::get_zone, this));
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id/schedule", Routes::bind(&API::get_zone_schedule, this));
    Routes::Get(router, "profiles/:profile_id/zones/:zone_id/leds", Routes::bind(&API::get_zone_leds, this));
    Routes::Put(router, "profiles/:profile_id/zones/:zone_id/leds/add/:led_id", Routes::bind(&API::put_zone_led, this));
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
    Routes::Patch(router, "leds/:id/edit", Routes::bind(&API::patch_led, this));
    Routes::Delete(router, "leds/:id/delete", Routes::bind(&API::delete_led, this));
    Routes::Delete(router, "leds/:led_id/controller/delete", Routes::bind(&API::delete_led_controller, this));

    // Controller routes
    Routes::Get(router, "controllers", Routes::bind(&API::get_controllers, this));
    Routes::Get(router, "controllers/:id", Routes::bind(&API::get_controller, this));
    Routes::Post(router, "controllers/add", Routes::bind(&API::post_controller, this));
    Routes::Patch(router, "controllers/:id/edit", Routes::bind(&API::patch_controller, this));
    Routes::Delete(router, "controllers/:id/delete", Routes::bind(&API::delete_controller, this));

    // LEDState routes
    Routes::Get(router, "led_states", Routes::bind(&API::get_led_states, this));
    Routes::Get(router, "led_states/:id", Routes::bind(&API::get_led_state, this));        
    Routes::Post(router, "led_states/add", Routes::bind(&API::post_led_state, this));
    Routes::Patch(router, "led_states/:id/edit", Routes::bind(&API::patch_led_state, this));
    Routes::Delete(router, "led_states/:id/delete", Routes::bind(&API::delete_led_state, this));

    // DailyState routes
    Routes::Get(router, "daily_states", Routes::bind(&API::get_daily_states, this));
    Routes::Get(router, "daily_states/:id", Routes::bind(&API::get_daily_state, this));
    Routes::Get(router, "daily_states/:id/led_states", Routes::bind(&API::get_daily_state_led_states, this));
    Routes::Put(router, "daily_states/:daily_state_id/led_states/add",
                        Routes::bind(&API::put_daily_state_led_state, this));
    Routes::Post(router, "daily_states/add", Routes::bind(&API::post_daily_state, this));
    Routes::Patch(router, "daily_states/:id/edit", Routes::bind(&API::patch_daily_state, this));
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
    // Date and time
    time_t tt;
    time(&tt);
    struct tm* ltm = localtime(&tt);
    char timeBuffer[80];
    strftime (timeBuffer, 80, "%c", ltm);
    
    // Print log
    std::cout << "[" << timeBuffer << "] "
              << Http::methodString(request.method()) << " "
              << request.resource()
              << std::endl;
}


// Controller endpoints (REST methods)

// System routes
void API::index(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    response.send(Http::Code::Ok, "Welcome to PlanteRGB!");
}
void API::system_restart(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    response.send(Http::Code::Ok, "Restarting");
}

// Actions
void API::api_shutdown(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    response.send(Http::Code::Ok, "Shutting down");
    shutdown();
}


// Profile CRUD routes
void API::get_profiles(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Json response MIME
    response.headers()
            .add<Http::Header::ContentType>(MIME(Application, Json));
    
    // Build JSON from profiles vector
    json j = json::array(); // Empty JSON array []
    std::vector<Profile*> profiles = InternalState::get_profiles();
    for (auto profile : profiles) {
        j.push_back(*profile);
    }

    // Send response
    response.send(Http::Code::Ok, j.dump());
}
void API::get_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(id);

    // Build JSON
    json j; 
    if (profile != 0){ j = *profile; }
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    
    // Send response
    response.send(code, j.dump());
}
void API::get_profile_zones(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(id);

    // Build JSON from zones vector
    json j = json::array(); // Empty JSON array []
    if (profile != 0) {
        std::vector<Zone*> zones = profile->get_zones();
        for (auto zone : zones) {
            j.push_back(*zone);
        }
    }
   
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    
    // Send response
    response.send(code, j.dump());
}
void API::get_current_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Data
    Profile* profile = InternalState::get_current_profile();

    // Build JSON
    json j;
    if (profile != 0){ j = *profile; }
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    
    // Send response
    response.send(code, j.dump());
}
void API::post_current_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(id);
    
    // Build JSON
    json j; 
    if (profile != 0) {
        InternalState::set_current_profile(profile);
        j = *profile;
    }
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    
    // Send response
    response.send(code, j.dump());
}
void API::post_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Decode JSON
    json j_in = json::parse(request.body());

    // Data
    Profile p = j_in;
    Profile* profile = new Profile(p);
    profile->set_id(DataParser::next_profile_id());
    InternalState::add_profile(profile);
    
    // Build response
    json j_out = json{{"id", profile->get_id()}};

    // Send response
    response.send(Http::Code::Ok, j_out.dump());
}
void API::post_profile_zone(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(id);

    // Response JSON
    json j_out;

    if (profile)
    {
        // Decode JSON
        json j_in = json::parse(request.body());

        // Data
        Zone z = j_in;
        Zone* zone = new Zone(z);
        Schedule* s = new Schedule();
        s->set_id(DataParser::next_schedule_id());
        zone->set_schedule(s);
        zone->set_id(DataParser::next_zone_id());
        profile->add_zone(zone);

        // Build response
        j_out = json{{"id", zone->get_id()}};
    }
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);

    // Send response
    response.send(code, j_out.dump());
}
void API::patch_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Decode JSON
    json j_in = json::parse(request.body());

    // Data
    Profile p = j_in;
    Profile* profile = InternalState::get_profile(id);
    
    // Build response
    json j_out;
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    if (profile != 0) {
        profile->copy(p);
        j_out = *profile;
    }

    // Send response
    response.send(code, j_out.dump());
}
void API::delete_profile_zone(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);

        if (zone) {
            profile->delete_zone(zone);
            free(zone);
            code = Http::Code::Ok;
        }
    }

    // Send response
    response.send(code, "");
}
void API::delete_profile(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto id = request.param(":id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(id);
    Http::Code code = (profile != 0 ? Http::Code::Ok : Http::Code::Not_Found);
    InternalState::delete_profile(profile);

    // Memory
    free(profile);

    // Send response
    response.send(code, "");
}

// Zone routes
void API::get_zone(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;
    json j_out;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);

        if (zone) {
            j_out = *zone;         
            code = Http::Code::Ok;
        }
    }

    // Send response
    response.send(code, j_out.dump());
}
void API::get_zone_schedule(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;
    json j_out;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);
        if (zone) {
            Schedule* schedule = zone->get_schedule();
            if (schedule) {
                j_out = *schedule;         
                code = Http::Code::Ok;
            }
        }
    }

    // Send response
    response.send(code, j_out.dump());
}
void API::get_zone_leds(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();


    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;
    json j_out = json::array(); // Empty JSON array []

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);
        if (zone) {
            std::vector<LED*> leds = zone->get_leds();
            for (auto led : leds) {
                json j = *led;
                j_out.push_back(j);
            }
            code = Http::Code::Ok;
        }
    }

    // Send response
    response.send(code, j_out.dump());
}
void API::put_zone_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();
    auto led_id = request.param(":led_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);
        if (zone) {
            LED* led = InternalState::get_led(led_id);
            if (led) {
                zone->add_led(led);
                code = Http::Code::Ok;
            }
        }
    }

    // Send response
    response.send(code, "");
}
void API::delete_zone_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();
    auto led_id = request.param(":led_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);
        if (zone) {
            LED* led = InternalState::get_led(led_id);
            if (led) {
                zone->delete_led(led);
                code = Http::Code::Ok;
            }
        }
    }

    // Send response
    response.send(code, "");
}

// Schedule routes
void API::get_schedule_active_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    
    // Parameters
    auto profile_id = request.param(":profile_id").as<unsigned int>();
    auto zone_id = request.param(":zone_id").as<unsigned int>();

    // Data
    Profile* profile = InternalState::get_profile(profile_id);
    Http::Code code = Http::Code::Not_Found;
    json j_out;

    if (profile) {
        Zone* zone = profile->get_zone(zone_id);
        if (zone) {
            Schedule* schedule = zone->get_schedule();
            if (schedule) {
                // Get time and day
                unsigned int time_of_day = 0;
                int day = 0;
                LEDState* l = schedule->get_active_state(time_of_day, day);
                if (l) {
                    j_out = *l;
                    code = Http::Code::Ok;
                }
            }
        }
    }

    // Send response
    response.send(code, j_out.dump());
}
void API::put_schedule_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_schedule_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// LED routes
void API::get_leds(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_led_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::patch_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// Controller routes
void API::get_controllers(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::patch_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_controller(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// LEDState routes
void API::get_led_states(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::patch_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}

// DailyState routes
void API::get_daily_states(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Json response MIME
    response.headers()
            .add<Http::Header::ContentType>(MIME(Application, Json));
    
    // Build JSON from profiles vector
    json j = json::array(); // Empty JSON array []
    std::vector<DailyState*> dailyStates = InternalState::get_daily_states();
    for (auto dailyState : dailyStates) {
        j.push_back(*dailyState);
    }

    // Send response
    response.send(Http::Code::Ok, j.dump());
}
void API::get_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::get_daily_state_led_states(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::put_daily_state_led_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::post_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);

    // Decode JSON
    json j_in = json::parse(request.body());

    // Data
    DailyState ds = j_in;
    ds.set_id(DataParser::next_daily_state_id());
    DailyState* dailyState = new DailyState(ds);
    dailyState->set_id(ds.get_id());
    InternalState::add_daily_state(dailyState);
    
    // Build response
    json j_out = json{{"id", ds.get_id()}};

    // Send response
    response.send(Http::Code::Ok, j_out.dump());
}
void API::patch_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_daily_state(REQUEST, RESPONSE)
{
    // Log request
    log_req(request);
    response.send(Http::Code::Ok, "Error: not Implemented");
}
void API::delete_daily_state_led_state(REQUEST, RESPONSE)
{
    // Log request
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
