// Include Guard
#ifndef __LEDSTATE_H_INCLUDED__
#define __LEDSTATE_H_INCLUDED__
//=================================

#include "./json.hpp"

using nlohmann::json;

class LEDState
{
public:
    // USE ACCESSORS AND MUTATORS
    unsigned int id;
    int r, g, b;
    int intensity;
    bool power;

    // Constructor
    LEDState();
    LEDState(const LEDState& ls);

    // Copy
    void copy(const LEDState& ls);

    // Accessors
    unsigned int get_id() const;
    int get_r() const;
    int get_g() const;
    int get_b() const;
    int get_intensity() const;
    bool get_power() const;

    // Mutators
    void set_id(unsigned int);
    void set_color(int r, int g, int b);
    void set_intensity(int);
    void set_power(bool);

    // Static states
    static LEDState* off;
};

// JSON
void to_json(json& j, const LEDState& ls);
void from_json(const json& j, LEDState& ls);


#endif //__CONTROLLER_H_INCLUDED__
