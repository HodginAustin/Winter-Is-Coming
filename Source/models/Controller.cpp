#include "../includes/Controller.hpp"

Controller::Controller(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int Controller::get_id()
{
    return id;
}


// Mutators
void Controller::set_id(unsigned int val)
{
    id = val;
}
