#include <string>

class Profile
{
private:
    unsigned int id;
    std::string name;
    std::string description;

public:
    // Constructor
    Profile(unsigned int id);

    // Accessors
    unsigned int get_id();
    std::string get_name();
    std::string get_description();

    // Mutators
    void set_id(unsigned int);
    void set_name(std::string);
    void set_description(std::string);
};
