#include "../includes/LEDState.hpp"


// Constructor
LEDState::LEDState(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int LEDState::get_id() const
{
    return id;
}

int LEDState::get_color() const
{
    return color;
}

float LEDState::get_intensity() const
{
    return intensity;
}

bool LEDState::get_power() const
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
