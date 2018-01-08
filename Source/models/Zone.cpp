#include "../includes/Zone.hpp"

Zone::Zone(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int Zone::get_id()
{
    return id;
}

unsigned int Zone::get_schedule_id()
{
    return schedule_id;
}


// Mutators
void Zone::set_id(unsigned int val)
{
    id = val;
}

void Zone::set_schedule_id(unsigned int val)
{
    schedule_id = val;
}
