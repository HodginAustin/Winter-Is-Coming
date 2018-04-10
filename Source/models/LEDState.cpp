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
const unsigned int& LEDState::get_id() const
{
    return this->id;
}

const int& LEDState::get_r() const { return this->r; }
const int& LEDState::get_g() const { return this->g; }
const int& LEDState::get_b() const { return this->b; }

const int& LEDState::get_intensity() const
{
    return this->intensity;
}

const bool& LEDState::get_power() const
{
    return this->power;
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
void LEDState::set_r(int r) { this->r = r; }
void LEDState::set_g(int g) { this->g = g; }
void LEDState::set_b(int b) { this->b = b; }

void LEDState::set_intensity(int val)
{
    intensity = val;
}

void LEDState::set_power(bool val)
{
    power = val;
}

// Equals
bool LEDState::equals(LEDState* lhs, LEDState* rhs)
{
    bool equal = true;

    equal &= (lhs->get_r() == rhs->get_r());
    equal &= (lhs->get_g() == rhs->get_g());
    equal &= (lhs->get_b() == rhs->get_b());
    equal &= (lhs->get_intensity() == rhs->get_intensity());
    equal &= (lhs->get_power() == rhs->get_power());

    return equal;
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
