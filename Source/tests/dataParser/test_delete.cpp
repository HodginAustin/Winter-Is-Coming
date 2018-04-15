#include "../asserting.h"
#include "../../includes/DataParser.hpp"
#include "../../includes/Profile.hpp"
#include "../../includes/Zone.hpp"
#include "../../includes/DailyState.hpp"
#include "../../includes/LEDState.hpp"
#include "../../includes/LED.hpp"

#include <iostream>
bool test_delete()
{
    // Initialize
    DataParser::initialize(true);
    DataParser::clear();

    /*
        static void remove(Profile*);
        static void remove(Zone*);
        static void remove(LED*);
        static void remove(LEDState*);
        static void remove(DailyState*);
        static void remove(Controller*);
        static void remove(Zone*, LED*);
        static void remove(Zone*, int);
        static void remove(DailyState*, int);
    */

    bool condTrue = true;
    bool condFalse = false;


    // PROFILE
   

    // ZONE
   


    // CONTROLLER
    


    // LED
    


    // DAILY STATE
   


    // LED STATE
    

    // Cleanup
    DataParser::clear();

    return condTrue && !condFalse;
}
