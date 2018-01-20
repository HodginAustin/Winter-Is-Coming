// Include Guard
#ifndef __ZONE_H_INCLUDED__
#define __ZONE_H_INCLUDED__
//=================================


#include <vector>
#include "./Schedule.hpp"
#include "./led.hpp"

class Zone
{
private:
    unsigned int id;
    Schedule* schedule;
    std::vector<LED*> leds;

public:
    // Constructor
    Zone(unsigned int id);

    // Accessors
    unsigned int get_id();
    Schedule* get_schedule();

    // Mutators
    void set_id(unsigned int);
    void set_schedule(Schedule*);

    // CRUD
    void add_led(LED* led);
    std::vector<LED*> get_leds();
    void delete_led(LED* led);
};

#endif //__ZONE_H_INCLUDED__
