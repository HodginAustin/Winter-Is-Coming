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

#include "./relationships.hpp"


#include "./sqlite_orm.hpp"

using namespace sqlite_orm;

class DataParser
{
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
    static void insert(ZoneDOW);
    static void insert(ZoneToLED);
    static void insert(DailyStateToLEDState);
    
    // UPDATE
    static void update(Profile*);
    static void update(Zone*);
    static void update(LED*);
    static void update(LEDState*);
    static void update(DailyState*);
    static void update(Controller*);

    // SELECT
    static bool get_all();

    // DELETE
    static void remove(Profile*);
    static void remove(Zone*);
    static void remove(LED*);
    static void remove(LEDState*);
    static void remove(DailyState*);
    static void remove(Controller*);
    static void remove(Zone*, LED*);
    static void remove(Zone*, int);

    static void clear();
};

#endif //__DATA_PARSER_H_INCLUDED__
