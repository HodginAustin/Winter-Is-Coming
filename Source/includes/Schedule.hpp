
class Schedule
{
private:
    unsigned int id;
    unsigned int effective_time;
    unsigned int led_state_id;

public:
    Schedule(unsigned int id);

    // Accessors
    unsigned int get_id();
    unsigned int get_effective_time();
    unsigned int get_led_state_id();

    // Mutators
    void set_id(unsigned int);
    void set_effective_time(unsigned int);
    void set_led_state_id(unsigned int);
};
