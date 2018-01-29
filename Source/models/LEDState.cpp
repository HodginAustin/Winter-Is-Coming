#include "../includes/LEDState.hpp"


// Static objects
LEDState* LEDState::off = new LEDState();


// Constructor
LEDState::LEDState() {
    r = g = b = intensity = power = 0;
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
    if (red >= 0) { r = red; }
    if (green >= 0) { g = green; }
    if (blue >= 0) { b = blue; }
}

void LEDState::set_intensity(float val)
{
    intensity = val;
}

void LEDState::set_power(bool val)
{
    power = val;
}


// JSON
void to_json(json& j, const LEDState& ls) {
    j = json{
        {"id", ls.get_id()},
        {"r", ls.get_r()},
        {"g", ls.get_g()},
        {"b", ls.get_b()},
        {"intensity", ls.get_intensity()},
        {"power", ls.get_power()},
    };
}

void from_json(const json& j, LEDState& ls) {
    int r = -1, g = -1, b = -1; // Initialize to bad value, 

    if (j.find("r") != j.end()){ r = j.at("r").get<int>(); }
    if (j.find("g") != j.end()){ g = j.at("g").get<int>(); }
    if (j.find("b") != j.end()){ b = j.at("b").get<int>(); }
    ls.set_color(r,g,b);

    if (j.find("intensity") != j.end()) {
        ls.set_intensity(j.at("intensity").get<int>());
    }
    if (j.find("power") != j.end()) {
        ls.set_power(j.at("power").get<bool>());
    }
}
