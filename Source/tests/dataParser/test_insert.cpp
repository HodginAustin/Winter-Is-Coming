#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include <iostream>
bool test_insert()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static unsigned int insert(Profile*);
        static unsigned int insert(Zone*);
        static unsigned int insert(LED*);
        static unsigned int insert(LEDState*);
        static unsigned int insert(DailyState*);
        static unsigned int insert(Controller*);
        static void insert(ZoneDOW);
        static void insert(ZoneToLED);
        static void insert(DailyStateToLEDState);
        static void insert(Setting setting);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    condTrue &= assertTrue(DataParser::selectProfile(1) == 0);
    unsigned int id = DataParser::insert(p);
    p->set_id(id);
    condTrue &= assertTrue(DataParser::selectProfile(id)->get_id() == p->get_id());

    Profile* p2 = new Profile();
    condTrue &= assertTrue(DataParser::selectProfile(1) != 0);
    unsigned int id2 = DataParser::insert(p2);
    p2->set_id(id2);
    condTrue &= assertTrue(DataParser::selectProfile(id2) != DataParser::selectProfile(id));
    

    // ZONE


    // LED


    // CONTROLLER


    // DAILY STATE


    // LED STATE


    // Zone DOW

    // ZoneToLED

    // DailyStateToLEDState

    
    //condTrue &= assertTrue(c->get_id() == 1);

    //condFalse |= assertFalse();

    return condTrue && !condFalse;
}
