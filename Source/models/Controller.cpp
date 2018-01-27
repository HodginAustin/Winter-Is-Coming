#include "../includes/Controller.hpp"


// Constructor
Controller::Controller() {}

Controller::Controller(const Controller& c)
{
    copy(c);
}


// Copy
void Controller::copy(const Controller& c)
{
    if (c.get_io() >= 0) {
        set_io(c.get_io());
    }
    if (!c.get_address().empty()) {
        set_address(c.get_address());
    }
    if (!c.get_details().empty()) {
        set_details(c.get_details());
    }
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
