// Include Guard
#ifndef __LED_H_INCLUDED__
#define __LED_H_INCLUDED__
//=================================

#include "./json.hpp"

#include "./Controller.hpp"

using nlohmann::json;

class LED
{
private:
    unsigned int id;
    unsigned int strip_idx;
    Controller* controller;

public:
    // USE ACCESSORS AND MUTATORS
    std::shared_ptr<unsigned int> controller_id;

    // Constructor
    LED();
    LED(const LED& l);

    // Copy
    void copy(const LED& l);

    // Accessors
    const unsigned int& get_id() const;
    const unsigned int& get_strip_idx() const;
    Controller* get_controller() const;

    // Mutators
    void set_id(unsigned int);
    void set_strip_idx(unsigned int);
    void set_controller(Controller*);
};

// JSON
void to_json(json& j, const LED& l);
void from_json(const json& j, LED& l);


#endif //__LED_H_INCLUDED__
