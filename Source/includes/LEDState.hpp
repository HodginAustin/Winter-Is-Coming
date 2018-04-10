// Include Guard
#ifndef __LEDSTATE_H_INCLUDED__
#define __LEDSTATE_H_INCLUDED__
//=================================

#include "./json.hpp"

using nlohmann::json;

class LEDState
{
private:
    unsigned int id;
    int r, g, b;
    int intensity;
    bool power;
public:

    // Constructor
    LEDState();
    LEDState(const LEDState& ls);

    // Copy
    void copy(const LEDState& ls);

    // Accessors
    const unsigned int& get_id() const;
    const int& get_r() const;
    const int& get_g() const;
    const int& get_b() const;
    const int& get_intensity() const;
    const bool& get_power() const;

    // Mutators
    void set_id(unsigned int);
    void set_color(int r, int g, int b);
    void set_r(int r);
    void set_g(int g);
    void set_b(int b);
    void set_intensity(int);
    void set_power(bool);

    // Static states
    static LEDState* off;

    // Equals
    static bool equals(LEDState*, LEDState*);
};

// JSON
void to_json(json& j, const LEDState& ls);
void from_json(const json& j, LEDState& ls);


#endif //__CONTROLLER_H_INCLUDED__
