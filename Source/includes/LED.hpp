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
    LED(unsigned int id);

    // Accessors
    unsigned int get_id();
    unsigned int get_strip_idx();
    Controller* get_controller();

    // Mutators
    void set_id(unsigned int);
    void set_strip_idx(unsigned int);
    void set_controller(Controller*);
};

#endif //__LED_H_INCLUDED__
