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

unsigned int Controller::get_io()
{
    return io;
}

std::string Controller::get_address()
{
    return address;
}

std::string Controller::get_details()
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
