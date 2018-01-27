#include "../includes/LEDState.hpp"


// Constructor
LEDState::LEDState() {
    r = g = b = intensity = power = -1;
}
LEDState::LEDState(const LEDState& ls)
{
    copy(ls);
}


// Copy
void LEDState::copy(const LEDState& ls)
{
    set_color(ls.get_r(), ls.get_g(), ls.get_b());
    if (ls.get_intensity() >= 0){
        set_intensity(ls.get_intensity());
    }
    set_power(ls.get_power());
}


// Accessors
unsigned int LEDState::get_id() const
{
    return id;
}

int LEDState::get_r() const { return r; }
int LEDState::get_g() const { return g; }
int LEDState::get_b() const { return b; }

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

void LEDState::set_color(int red, int green, int blue)
{
    r = red; g = green; b = blue;
}

void LEDState::set_intensity(float val)
{
    intensity = val;
}

void LEDState::set_power(bool val)
{
    power = val;
}
