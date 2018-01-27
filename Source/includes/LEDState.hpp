// Include Guard
#ifndef __LEDSTATE_H_INCLUDED__
#define __LEDSTATE_H_INCLUDED__
//=================================


class LEDState
{
private:
    unsigned int id;
    int color;
    float intensity;
    bool power;

public:
    // Constructor
    LEDState(unsigned int id);

    // Accessors
    unsigned int get_id() const;
    int get_color() const;
    float get_intensity() const;
    bool get_power() const;

    // Mutators
    void set_id(unsigned int);
    void set_color(int);
    void set_intensity(float);
    void set_power(bool);
};

#endif //__CONTROLLER_H_INCLUDED__
