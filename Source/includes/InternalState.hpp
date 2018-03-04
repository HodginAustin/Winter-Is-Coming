// Include Guard
#ifndef __INTERNALSTATE_H_INCLUDED__
#define __INTERNALSTATE_H_INCLUDED__
//=================================


#include <vector>
#include "./Profile.hpp"
#include "./Zone.hpp"
#include "./LED.hpp"
#include "./Controller.hpp"
#include "./LEDState.hpp"
#include "./DailyState.hpp"

class InternalState
{
private:
    // Abstract relationship objects
    static std::vector<Profile*> profiles;
    static Profile* currentProfile;
    // Physically based objects
    static std::vector<LED*> leds;
    static std::vector<Controller*> controllers;
    // State objects
    static std::vector<LEDState*> ledStates;
    static std::vector<DailyState*> dailyStates;
public:
    // Initialization
    static bool initialize();

    // Accessors
    static Profile* get_current_profile();

    // Mutators
    static void set_current_profile(Profile* p);

    // Profile CRUD
    static void add_profile(Profile* profile);
    static std::vector<Profile*> get_profiles();
    static Profile* get_profile(unsigned int id);
    static void delete_profile(Profile* profile);
    // LED CRUD
    static void add_led(LED* led);
    static std::vector<LED*> get_leds();
    static LED* get_led(unsigned int id);
    static void delete_led(LED* led);
    // Controller CRUD
    static void add_controller(Controller* controller);
    static std::vector<Controller*> get_controllers();
    static Controller* get_controller(unsigned int id);
    static void delete_controller(Controller* controller);
    // LEDState CRUD
    static void add_led_state(LEDState* ledState);
    static std::vector<LEDState*> get_led_states();
    static LEDState* get_led_state(unsigned int id);
    static void delete_led_state(LEDState* ledState);
    // DailyState CRUD
    static void add_daily_state(DailyState* dailyState);
    static std::vector<DailyState*> get_daily_states();
    static DailyState* get_daily_state(unsigned int id);
    static void delete_daily_state(DailyState* dailyState);

    // Clear
    static void clear();
    
    // Helpers
    static unsigned int get_time();
    static int get_day();
};

#endif //__INTERNALSTATE_H_INCLUDED__
