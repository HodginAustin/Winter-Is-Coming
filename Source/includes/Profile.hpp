// Include Guard
#ifndef __PROFILE_H_INCLUDED__
#define __PROFILE_H_INCLUDED__
//=================================


#include <string>
#include <vector>
#include "./Zone.hpp"

class Profile
{
private:
    unsigned int id;
    std::string name;
    std::string description;
    std::vector<Zone*> zones;

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

    // CRUD
    void add_zone(Zone* newZone);
    std::vector<Zone*> get_zones();
    void delete_zone(Zone* zone);
};

#endif //__PROFILE_H_INCLUDED__
