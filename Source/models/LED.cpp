#include "../includes/LED.hpp"


// Constructor
LED::LED() {}

LED::LED(const LED& l)
{
    copy(l);
}


// Copy
void LED::copy(const LED& l)
{
    if (l.get_strip_idx() >= 0){
        set_strip_idx(l.get_strip_idx());
    }
    if (l.get_controller() != 0){
        set_controller(l.get_controller());
    }
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
