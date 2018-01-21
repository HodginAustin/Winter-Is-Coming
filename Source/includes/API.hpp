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
    // first part of function name is HTTP verb
    // GET retrieves an object and group of objects globally or from a container
    // POST adds an object to the system and links it to its parent container
    // PUT links an already added object to its parent container

    // System routes
    void system_restart(REQUEST_RESPONSE);

    // Actions
    void api_shutdown(REQUEST_RESPONSE);

    // Top level objects (get all of a specific object without knowing what it belongs to)
    // add new objects which are not linked to any other objects yet
    void get_profiles(REQUEST_RESPONSE);          // Gets all profiles
    void post_profile(REQUEST_RESPONSE);          // Adds a profile
    void delete_profile(REQUEST_RESPONSE);        // Deletes a profile
    // LED CRUD
    void get_leds(REQUEST_RESPONSE);              // Gets all LEDs
    void post_led(REQUEST_RESPONSE);              // Adds an LED
    void delete_led(REQUEST_RESPONSE);            // Deletes an LED
    // Controller CRUD
    void get_controllers(REQUEST_RESPONSE);       // Gets all controllers
    void post_controller(REQUEST_RESPONSE);       // Adds a controller
    void delete_controller(REQUEST_RESPONSE);     // Deletes a controller
    // LEDState CRUD
    void get_led_states(REQUEST_RESPONSE);        // Gets all LED states
    void post_led_state(REQUEST_RESPONSE);        // Adds an LED state
    void delete_led_state(REQUEST_RESPONSE);      // Deletes an LED state
    // DailyState CRUD
    void get_daily_states(REQUEST_RESPONSE);      // Gets all daily states
    void post_daily_state(REQUEST_RESPONSE);      // Adds a daily state
    void delete_daily_state(REQUEST_RESPONSE);    // Deletes a daily state

    // Profile routes
    void get_profile(REQUEST_RESPONSE);           // Gets a profile by ID
    void get_current_profile(REQUEST_RESPONSE);   // Gets the active profile
    void get_profile_zones(REQUEST_RESPONSE);     // Gets a profile's zones
    void post_profile_zone(REQUEST_RESPONSE);     // Adds a zone, links to a profile
    void delete_profile_zone(REQUEST_RESPONSE);   // Deletes a zone, unlinks from a profile

    // Zone routes
    void get_zone(REQUEST_RESPONSE);              // Gets a zone by ID
    void get_zone_schedule(REQUEST_RESPONSE);     // Gets a zone's schedule (includes its 7 dailyStates)
    void get_zone_leds(REQUEST_RESPONSE);         // Gets a zone's LEDs
    void put_zone_led(REQUEST_RESPONSE);          // Links an existing LED to a zone
    void delete_zone_led(REQUEST_RESPONSE);       // Unlinks an existing LED from a zone

    // LED routes
    void get_led(REQUEST_RESPONSE);               // Gets an LED by ID
    void get_led_controller(REQUEST_RESPONSE);    // Gets an LED's controller
    void put_led_controller(REQUEST_RESPONSE);    // Links an existing controller to an LED
    void delete_led_controller(REQUEST_RESPONSE); // Unlinks an existing controller from an LED

    // Controller routes
    void get_controller(REQUEST_RESPONSE);        // Gets a controller by ID

    // Schedule routes
    void put_schedule_daily_state(REQUEST_RESPONSE);      // Links an existing dailyState to a schedule by day of week
    void delete_schedule_daily_state(REQUEST_RESPONSE);   // Unlinks an existing dailyState to a schedule
    void get_schedule_active_led_state(REQUEST_RESPONSE); // Gets the currently active LEDState based on the system time

    // DailyState routes
    void get_daily_state(REQUEST_RESPONSE);              // Gets a dailyState by ID
    void get_daily_state_led_states(REQUEST_RESPONSE);   // Gets a dailyState's LED states
    void put_daily_state_led_state(REQUEST_RESPONSE);    // Links an existing LEDState to a dailyState
    void delete_daily_state_led_state(REQUEST_RESPONSE); // Unlinks an existing LEDState from a dailyState

    // LEDState routes
    void get_led_state(get_time_state_count);     // Gets an LEDState by ID
};

#endif //__API_H_INCLUDED__
