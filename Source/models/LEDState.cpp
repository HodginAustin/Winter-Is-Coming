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

int LEDState::get_color()
{
    return color;
}

float LEDState::get_intensity()
{
    return intensity;
}

bool LEDState::get_power()
{
    return power;
}


// Mutators
void LEDState::set_id(unsigned int val)
{
    id = val;
}

void LEDState::set_color(int val)
{
    color = val;
}

void LEDState::set_intensity(float val)
{
    intensity = val;
}

void LEDState::set_power(bool val)
{
    power = val;
}
