// Include Guard
#ifndef __CONTROLLER_H_INCLUDED__
#define __CONTROLLER_H_INCLUDED__
//=================================


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
    Controller();
    Controller(const Controller& c);

    // Copy
    void copy(const Controller& c);

    // Accessors
    unsigned int get_id() const;
    unsigned int get_io() const;
    std::string get_address() const;
    std::string get_details() const;

    // Mutators
    void set_id(unsigned int);
    void set_io(unsigned int);
    void set_address(std::string);
    void set_details(std::string);
};

#endif //__CONTROLLER_H_INCLUDED__
