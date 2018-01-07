#include "../includes/Schedule.hpp"

Schedule::Schedule(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int Schedule::get_id()
{
    return id;
}


// Mutators
void Schedule::set_id(unsigned int val)
{
    id = val;
}
