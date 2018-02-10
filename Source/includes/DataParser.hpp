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


#include "./sqlite_orm.hpp"

using namespace sqlite_orm;

class DataParser
{
private:
    static unsigned int profile_id;
    static unsigned int zone_id;
    static unsigned int led_id;
    static unsigned int led_state_id;
    static unsigned int daily_state_id;
    static unsigned int controller_id;

public:
    // Initialization
    static bool initialize();

    // INSERT
    static unsigned int insert(Profile*);
    static unsigned int insert(Zone*);
    static unsigned int insert(LED*);
    static unsigned int insert(LEDState*);
    static unsigned int insert(DailyState*);
    static unsigned int insert(Controller*);
    
    // UPDATE
    static void update(Profile*);
    static void update(Zone*);
    static void update(LED*);
    static void update(LEDState*);
    static void update(DailyState*);
    static void update(Controller*);

    // SELECT
    static void get_all();

    // DELETE
    static void remove(Profile*);
    static void remove(Zone*);
    static void remove(LED*);
    static void remove(LEDState*);
    static void remove(DailyState*);
    static void remove(Controller*);
};

#endif //__DATA_PARSER_H_INCLUDED__
