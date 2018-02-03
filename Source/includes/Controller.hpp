// Include Guard
#ifndef __CONTROLLER_H_INCLUDED__
#define __CONTROLLER_H_INCLUDED__
//=================================


#include <string>

#include "./json.hpp"

using nlohmann::json;

class Controller
{
public:
    // USE ACCESSORS AND MUTATORS
    unsigned int id;
    unsigned int io;
    std::string address;
    std::string details;

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

// JSON
void to_json(json& j, const Controller& c);
void from_json(const json& j, Controller& c);


#endif //__CONTROLLER_H_INCLUDED__
