#include "./asserting.h"
#include "../includes/Controller.hpp"

bool test_controller()
{
    Controller* c = new Controller(1);

    bool condTrue = true;
    bool condFalse = false;
    
    condTrue &= assertTrue(c->get_id() == 1);

    c->set_address("Hello");
    condTrue &= assertTrue(c->get_address()=="Hello");

    //condFalse |= assertFalse();

    return condTrue && !condFalse;
}