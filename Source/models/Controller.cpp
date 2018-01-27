#include "../includes/Controller.hpp"


// Constructor
Controller::Controller(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int Controller::get_id() const
{
    return id;
}

unsigned int Controller::get_io() const
{
    return io;
}

std::string Controller::get_address() const
{
    return address;
}

std::string Controller::get_details() const
{
    return details;
}


// Mutators
void Controller::set_id(unsigned int val)
{
    id = val;
}

void Controller::set_io(unsigned int val)
{
    io = val;
}

void Controller::set_address(std::string val)
{
    address = val;
}

void Controller::set_details(std::string val)
{
    details = val;
}
