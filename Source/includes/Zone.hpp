// Include Guard
#ifndef __ZONE_H_INCLUDED__
#define __ZONE_H_INCLUDED__
//=================================


#include <vector>
#include "./Schedule.hpp"
#include "./LED.hpp"

class Zone
{
private:
    unsigned int id;
    Schedule* schedule;
    std::vector<LED*> leds;

public:
    // Constructor
    Zone();
    Zone(const Zone& z);

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

// JSON
void to_json(json& j, const Zone& p);
void from_json(const json& j, Zone& p);


#endif //__ZONE_H_INCLUDED__
