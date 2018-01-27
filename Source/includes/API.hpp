// Include Guard
#ifndef __API_H_INCLUDED__
#define __API_H_INCLUDED__
//=================================

#include <pistache/endpoint.h>
#include <pistache/router.h>


#define REQUEST const Pistache::Rest::Request& request
#define RESPONSE Pistache::Http::ResponseWriter response


using namespace Pistache;


class API {
public:
    API(Address addr);

    static bool initialize(size_t thr);
    static void start();
    static void shutdown();

private:
    static API* instance;
    static std::shared_ptr<Http::Endpoint> httpEndpoint;
    static Rest::Router router;

    static volatile bool api_power_state = true;

    static void setup_routes();

    static void log_req(REQUEST);


    // Controller functions (REST methods)
    // first part of function name is HTTP verb
    // GET retrieves an object and group of objects globally or from a container
    // POST adds an object to the system and links it to its parent container
    // PUT links an already added object to its parent container
    // PATCH for an existing item will update that item

    // System routes
    static void index(REQUEST, RESPONSE);
    static void system_restart(REQUEST, RESPONSE);

    // API Actions
    static void api_shutdown(REQUEST, RESPONSE);

    // Top level objects (get all of a specific object without knowing what it belongs to)
    // add new objects which are not linked to any other objects yet

    // Profile routes
    static void get_profiles(REQUEST, RESPONSE);          // Gets all profiles
    static void get_profile(REQUEST, RESPONSE);           // Gets a profile by ID
    static void get_profile_zones(REQUEST, RESPONSE);     // Gets a profile's zones
    static void get_current_profile(REQUEST, RESPONSE);   // Gets the active profile
    static void post_profile(REQUEST, RESPONSE);          // Adds a profile
    static void post_profile_zone(REQUEST, RESPONSE);     // Adds a zone, links to a profile
    static void patch_profile(REQUEST, RESPONSE);         // Updates a profile
    static void delete_profile_zone(REQUEST, RESPONSE);   // Deletes a zone, unlinks from a profile
    static void delete_profile(REQUEST, RESPONSE);        // Deletes a profile

    // Zone routes
    static void get_zone(REQUEST, RESPONSE);              // Gets a zone by ID
    static void get_zone_schedule(REQUEST, RESPONSE);     // Gets a zone's schedule (includes its 7 dailyStates)
    static void get_zone_leds(REQUEST, RESPONSE);         // Gets a zone's LEDs
    static void put_zone_led(REQUEST, RESPONSE);          // Links an existing LED to a zone
    static void delete_zone_led(REQUEST, RESPONSE);       // Unlinks an existing LED from a zone

    // Schedule routes
    static void get_schedule_active_led_state(REQUEST, RESPONSE); // Gets the currently active LEDState based on the system time
    static void put_schedule_daily_state(REQUEST, RESPONSE);      // Links an existing dailyState to a schedule by day of week
    static void delete_schedule_daily_state(REQUEST, RESPONSE);   // Unlinks an existing dailyState to a schedule

    // LED routes
    static void get_leds(REQUEST, RESPONSE);              // Gets all LEDs
    static void get_led(REQUEST, RESPONSE);               // Gets an LED by ID
    static void get_led_controller(REQUEST, RESPONSE);    // Gets an LED's controller
    static void put_led_controller(REQUEST, RESPONSE);    // Links an existing controller to an LED
    static void post_led(REQUEST, RESPONSE);              // Adds an LED
    static void patch_led(REQUEST, RESPONSE);             // Updates an LED
    static void delete_led(REQUEST, RESPONSE);            // Deletes an LED
    static void delete_led_controller(REQUEST, RESPONSE); // Unlinks an existing controller from an LED

    // Controller CRUD routes
    static void get_controllers(REQUEST, RESPONSE);       // Gets all controllers
    static void get_controller(REQUEST, RESPONSE);        // Gets a controller by ID
    static void post_controller(REQUEST, RESPONSE);       // Adds a controller
    static void patch_controller(REQUEST, RESPONSE);      // Updates a controller
    static void delete_controller(REQUEST, RESPONSE);     // Deletes a controller

    // LEDState CRUD routes
    static void get_led_states(REQUEST, RESPONSE);        // Gets all LED states
    static void get_led_state(REQUEST, RESPONSE);         // Gets an LEDState by ID
    static void post_led_state(REQUEST, RESPONSE);        // Adds an LED state
    static void patch_led_state(REQUEST, RESPONSE);       // Adds an LED state
    static void delete_led_state(REQUEST, RESPONSE);      // Deletes an LED state

    // DailyState CRUD routes
    static void get_daily_states(REQUEST, RESPONSE);      // Gets all daily states
    static void get_daily_state(REQUEST, RESPONSE);              // Gets a dailyState by ID
    static void get_daily_state_led_states(REQUEST, RESPONSE);   // Gets a dailyState's LED states
    static void put_daily_state_led_state(REQUEST, RESPONSE);    // Links an existing LEDState to a dailyState
    static void post_daily_state(REQUEST, RESPONSE);      // Adds a daily state
    static void patch_daily_state(REQUEST, RESPONSE);     // Updates a daily state
    static void delete_daily_state(REQUEST, RESPONSE);    // Deletes a daily state
    static void delete_daily_state_led_state(REQUEST, RESPONSE); // Unlinks an existing LEDState from a dailyState
};

#endif //__API_H_INCLUDED__
