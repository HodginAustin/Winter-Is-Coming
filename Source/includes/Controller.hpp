#include <string>

class Controller
{
private:
    unsigned int id;
    unsigned int io;
    std::string address;
    std::string details;

public:
    // Constructor
    Controller(unsigned int id);

    // Accessors
    unsigned int get_id();
    unsigned int get_io();
    std::string get_address();
    std::string get_details();

    // Mutators
    void set_id(unsigned int);
    void set_io(unsigned int);
    void set_address(std::string);
    void set_details(std::string);
};
