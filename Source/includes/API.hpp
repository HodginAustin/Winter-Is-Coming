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

    bool initialize(size_t thr);
    void start();
    void shutdown();

private:
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    volatile bool api_power_state = true;

    void setup_routes();

    void log_req(REQUEST);


    // Controller functions (REST methods)
    // first part of function name is HTTP verb
    // GET retrieves an object and group of objects globally or from a container
    // POST adds an object to the system and links it to its parent container
    // PUT links an already added object to its parent container
    // PATCH for an existing item will update that item

    // System routes
    void index(REQUEST, RESPONSE);
    void system_shutdown(REQUEST, RESPONSE);
    void system_restart(REQUEST, RESPONSE);

    // API Actions
    void api_shutdown(REQUEST, RESPONSE);
    void nuke_from_orbit(REQUEST, RESPONSE);

    // Top level objects (get all of a specific object without knowing what it belongs to)
    // add new objects which are not linked to any other objects yet

    // Profile routes
    void get_profiles(REQUEST, RESPONSE);          // Gets all profiles
    void get_profile(REQUEST, RESPONSE);           // Gets a profile by ID
    void get_profile_zones(REQUEST, RESPONSE);     // Gets a profile's zones
    void get_current_profile(REQUEST, RESPONSE);   // Gets the active profile
    void post_current_profile(REQUEST, RESPONSE);  // Sets the active profile
    void post_profile(REQUEST, RESPONSE);          // Adds a profile
    void post_profile_zone(REQUEST, RESPONSE);     // Adds a zone, links to a profile
    void patch_profile(REQUEST, RESPONSE);         // Updates a profile
    void delete_profile_zone(REQUEST, RESPONSE);   // Deletes a zone, unlinks from a profile
    void delete_profile(REQUEST, RESPONSE);        // Deletes a profile

    // Zone routes
    void get_zone(REQUEST, RESPONSE);              // Gets a zone by ID
    void get_zone_leds(REQUEST, RESPONSE);         // Gets a zone's LEDs
    void get_zone_active_led_state(REQUEST, RESPONSE); // Gets the currently active LEDState based on the system time
    void put_zone_led(REQUEST, RESPONSE);          // Links a list of existing LEDs to a zone
                                                        // IN: list of LED IDs [1,4,9,40]
                                                        // OUT: list of things that were not found, or nothing on success
    void put_zone_daily_state(REQUEST, RESPONSE);      // Links an existing dailyState to a zone by day of week
    void delete_zone_led(REQUEST, RESPONSE);       // Unlinks an existing LED from a zone
    void delete_zone_daily_state(REQUEST, RESPONSE);   // Unlinks an existing dailyState to a zone

    // LED routes
    void get_leds(REQUEST, RESPONSE);              // Gets all LEDs
    void get_led(REQUEST, RESPONSE);               // Gets an LED by ID
    void get_led_controller(REQUEST, RESPONSE);    // Gets an LED's controller
    void put_led_controller(REQUEST, RESPONSE);    // Links an existing controller to an LED
    void post_led(REQUEST, RESPONSE);              // Adds an LED
    void patch_led(REQUEST, RESPONSE);             // Updates an LED
    void delete_led(REQUEST, RESPONSE);            // Deletes an LED
    void delete_led_controller(REQUEST, RESPONSE); // Unlinks an existing controller from an LED

    // Controller CRUD routes
    void get_controllers(REQUEST, RESPONSE);       // Gets all controllers
    void get_controller(REQUEST, RESPONSE);        // Gets a controller by ID
    void post_controller(REQUEST, RESPONSE);       // Adds a controller
    void patch_controller(REQUEST, RESPONSE);      // Updates a controller
    void delete_controller(REQUEST, RESPONSE);     // Deletes a controller

    // LEDState CRUD routes
    void get_led_states(REQUEST, RESPONSE);        // Gets all LED states
    void get_led_state(REQUEST, RESPONSE);         // Gets an LEDState by ID
    void post_led_state(REQUEST, RESPONSE);        // Adds an LED state
    void patch_led_state(REQUEST, RESPONSE);       // Adds an LED state
    void delete_led_state(REQUEST, RESPONSE);      // Deletes an LED state

    // DailyState CRUD routes
    void get_daily_states(REQUEST, RESPONSE);      // Gets all daily states
    void get_daily_state(REQUEST, RESPONSE);              // Gets a dailyState by ID
    void get_daily_state_led_states(REQUEST, RESPONSE);   // Gets a dailyState's LED states
    void put_daily_state_led_state(REQUEST, RESPONSE);    // Links an existing LEDState to a dailyState
                                                                // IN: list of objects containing time and state [{"time":8000,"state":5},{"time":23003,"state":1}]
                                                                // OUT: list of things that were not found, or nothing on success
    void post_daily_state(REQUEST, RESPONSE);      // Adds a daily state
    void patch_daily_state(REQUEST, RESPONSE);     // Updates a daily state
    void delete_daily_state(REQUEST, RESPONSE);    // Deletes a daily state
    void delete_daily_state_led_state(REQUEST, RESPONSE); // Unlinks an existing LEDState from a dailyState
};

#endif //__API_H_INCLUDED__
