#include "../includes/LED.hpp"


// Constructor
LED::LED(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int LED::get_id() const
{
    return id;
}

unsigned int LED::get_strip_idx() const
{
    return strip_idx;
}

Controller* LED::get_controller() const
{
    return controller;
}


// Mutators
void LED::set_id(unsigned int val)
{
    id = val;
}

void LED::set_strip_idx(unsigned int val)
{
    strip_idx = val;
}

void LED::set_controller(Controller* newController)
{
    controller = newController;
}
