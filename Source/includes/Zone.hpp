
class Zone
{
private:
    unsigned int id;
    unsigned int schedule_id;

public:
    Zone(unsigned int id);

    // Accessors
    unsigned int get_id();
    unsigned int get_schedule_id();

    // Mutators
    void set_id(unsigned int);
    void set_schedule_id(unsigned int);
};
