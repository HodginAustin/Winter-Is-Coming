#include <algorithm>
#include "../includes/Profile.hpp"


// Constructor
Profile::Profile(unsigned int id)
{
    set_id(id);
}


// Accessors
unsigned int Profile::get_id()
{
    return id;
}

std::string Profile::get_name()
{
    return name;
}

std::string Profile::get_description()
{
    return description;
}


// Mutators
void Profile::set_id(unsigned int val)
{
    id = val;
}

void Profile::set_name(std::string val)
{
    name = val;
}

void Profile::set_description(std::string val)
{
    description = val;
}


// CRUD
void Profile::add_zone(Zone* zone)
{
    zones.push_back(zone);
}

std::vector<Zone*> Profile::get_zones()
{
    return zones;
}

void Profile::delete_zone(Zone* zone)
{
    zones.erase(std::remove(zones.begin(), zones.end(), zone), zones.end());
}
