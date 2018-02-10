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
    // Constructor
    LED();
    LED(const LED& l);

    // Copy
    void copy(const LED& l);

    // Accessors
    const unsigned int& get_id() const;
    const unsigned int& get_strip_idx() const;
    Controller* get_controller() const;
    const unsigned int& get_controller_id() const;

    // Mutators
    void set_id(unsigned int);
    void set_strip_idx(unsigned int);
    void set_controller(Controller*);
    void set_controller_id(unsigned int id);
};

// JSON
void to_json(json& j, const LED& l);
void from_json(const json& j, LED& l);


#endif //__LED_H_INCLUDED__
