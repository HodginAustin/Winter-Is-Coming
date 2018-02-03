// Include Guard
#ifndef __DATA_PARSER_H_INCLUDED__
#define __DATA_PARSER_H_INCLUDED__
//=================================


#include <vector>
#include "./Profile.hpp"
#include "./Zone.hpp"
#include "./LED.hpp"
#include "./Controller.hpp"
#include "./LEDState.hpp"
#include "./DailyState.hpp"

class DataParser
{
private:
    static unsigned int profile_id;
    static unsigned int zone_id;
    static unsigned int schedule_id;
    static unsigned int led_id;
    static unsigned int led_state_id;
    static unsigned int daily_state_id;
    static unsigned int controller_id;

    // Database object
    storage_t<Ts...> storage;

public:
    // Initialization
    static bool initialize();

    static storage_t<Ts...> make_storage();

    // Accessors
    static unsigned int next_profile_id();
    static unsigned int next_zone_id();
    static unsigned int next_schedule_id();
    static unsigned int next_led_id();
    static unsigned int next_led_state_id();
    static unsigned int next_daily_state_id();
    static unsigned int next_controller_id();
};

#endif //__DATA_PARSER_H_INCLUDED__
