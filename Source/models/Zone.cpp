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


// Mutators
void Zone::set_id(unsigned int val)
{
    id = val;
}
