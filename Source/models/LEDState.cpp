#include "../includes/LEDState.hpp"

LEDState::LEDState(unsigned int id)
{
    set_id(id);
}

// Accessors
unsigned int LEDState::get_id()
{
    return id;
}


// Mutators
void LEDState::set_id(unsigned int val)
{
    id = val;
}
