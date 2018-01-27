// Include Guard
#ifndef __LED_H_INCLUDED__
#define __LED_H_INCLUDED__
//=================================


#include "./Controller.hpp"

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
    unsigned int get_id() const;
    unsigned int get_strip_idx() const;
    Controller* get_controller() const;

    // Mutators
    void set_id(unsigned int);
    void set_strip_idx(unsigned int);
    void set_controller(Controller*);
};

#endif //__LED_H_INCLUDED__
