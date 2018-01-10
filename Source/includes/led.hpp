
class LED
{
private:
    unsigned int id;
    unsigned int strip_idx;
    unsigned int controller_idx;
    unsigned int zone_id;
    unsigned int profile_id;

public:
    LED(unsigned int id);

    // Accessors
    unsigned int get_id();
    unsigned int get_strip_idx();
    unsigned int get_controller_idx();
    unsigned int get_zone_id();
    unsigned int get_profile_id();

    // Mutators
    void set_id(unsigned int);
    void set_strip_idx(unsigned int);
    void set_controller_idx(unsigned int);
    void set_zone_id(unsigned int);
    void set_profile_id(unsigned int);
};
