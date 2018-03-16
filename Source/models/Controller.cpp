#include "../includes/Controller.hpp"


// Constructor
Controller::Controller() {
    io = 1;
    address = "";
    details = "";
}

Controller::Controller(const Controller& c)
{
    copy(c);
}


// Copy
void Controller::copy(const Controller& c)
{
    if (c.get_io() > 0) {
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
const unsigned int& Controller::get_id() const
{
    return this->id;
}

const unsigned int& Controller::get_io() const
{
    return this->io;
}

const std::string& Controller::get_address() const
{
    return this->address;
}

const std::string& Controller::get_details() const
{
    return this->details;
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


// JSON
void to_json(json& j, const Controller& c) {    
    j = json{
        {"id", c.get_id()},
        {"io", c.get_io()},
        {"address", c.get_address()},
        {"details", c.get_details()}
    };
}

void from_json(const json& j, Controller& c) {
    if (j.find("io") != j.end()){
        c.set_io(j.at("io").get<unsigned int>());
    }
    if (j.find("address") != j.end()){
        c.set_address(j.at("address").get<std::string>());
    }
    if (j.find("details") != j.end()){
        c.set_details(j.at("details").get<std::string>());
    }
}
