
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
    unsigned int get_id();
    int get_color();
    float get_intensity();
    bool get_power();

    // Mutators
    void set_id(unsigned int);
    void set_color(int);
    void set_intensity(float);
    void set_power(bool);
};
