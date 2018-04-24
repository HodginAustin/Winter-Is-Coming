#include "../includes/LED.hpp"

#include "../includes/InternalState.hpp"


// Constructor
LED::LED() {
    id = 0;
    strip_idx = 0;
    controller = 0;
    controller_id = 0;
}

LED::LED(const LED& l) : LED()
{
    copy(l);
}

// Copy
void LED::copy(const LED& l)
{
    if (l.get_strip_idx() >= 0){
        set_strip_idx(l.get_strip_idx());
    }
    if (l.get_controller_id() > 0){
        set_controller_id(l.get_controller_id());
    }
}


// Accessors
const unsigned int& LED::get_id() const
{
    return this->id;
}

const unsigned int& LED::get_strip_idx() const
{
    return this->strip_idx;
}

Controller* LED::get_controller() const
{
    return controller;
}

const unsigned int& LED::get_controller_id() const
{
    return controller_id;
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
    set_controller_id(controller->get_id());
}

void LED::set_controller_id(unsigned int id)
{
    controller_id = id;
}


// JSON
void to_json(json& j, const LED& l) {
    Controller* c = l.get_controller();
    unsigned int c_id = (c ? c->get_id() : 0);

    j = json{
        {"id", l.get_id()},
        {"strip_idx", l.get_strip_idx()},
        {"controller", c_id},
    };
}

void from_json(const json& j, LED& l) {
    if (j.find("strip_idx") != j.end()){
        l.set_strip_idx(j.at("strip_idx").get<unsigned int>());
    }

    // Not optional parameter
    unsigned int id = j.at("controller").get<unsigned int>();
    l.set_controller(InternalState::get_controller(id));
}
