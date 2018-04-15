#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"

#include <iostream>
bool test_update()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static void update(Profile*);
        static void update(Zone*);
        static void update(LED*);
        static void update(LEDState*);
        static void update(DailyState*);
        static void update(Controller*);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
    Profile* p = new Profile();
    p->set_name("ptest");
    condTrue &= assertTrue(DataParser::selectProfile(1) == 0);
    unsigned int pid = DataParser::insert(p);
    p->set_id(pid);
    p->set_name("ptest2");
    DataParser::update(p);
    condTrue &= assertTrue(DataParser::selectProfile(pid)->get_name() == p->get_name());


    // ZONE
   


    // CONTROLLER
    


    // LED
    


    // DAILY STATE
   


    // LED STATE
    

    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
