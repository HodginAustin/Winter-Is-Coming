// Include Guard
#ifndef __LEDSTATE_H_INCLUDED__
#define __LEDSTATE_H_INCLUDED__
//=================================


class LEDState
{
private:
    unsigned int id;
    int r, g, b;
    float intensity;
    bool power;

public:
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
    float get_intensity() const;
    bool get_power() const;

    // Mutators
    void set_id(unsigned int);
    void set_color(int r, int g, int b);
    void set_intensity(float);
    void set_power(bool);
};

#endif //__CONTROLLER_H_INCLUDED__
