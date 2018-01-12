#include <map>

class DailyState
{
private:
    unsigned int id;
    std::map<unsigned int, unsigned int> hourlyStates;

public:
    // Constructor
    DailyState(unsigned int id);

    // Accessors
    unsigned int get_id();
    std::map<unsigned int, unsigned int> get_hourly_state_map();

    // Mutators
    void set_id(unsigned int);

    // CRUD
    bool create_update_state(unsigned int time, unsigned int state);
    unsigned int get_led_state_id(unsigned int time);
    bool delete_state(unsigned int time);
};
