#include <algorithm>
#include "../includes/Profile.hpp"


// Constructor
Profile::Profile() {}
Profile::Profile(const Profile& p) { copy(p); }


// Copy
void Profile::copy(const Profile& p)
{
    set_name(p.get_name());
    set_description(p.get_description());
}


// Accessors
unsigned int Profile::get_id() const
{
    return id;
}

std::string Profile::get_name() const
{
    return name;
}

std::string Profile::get_description() const
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


// JSON
void to_json(json& j, const Profile& p) {
    j = json{
        {"id", p.get_id()},
        {"name", p.get_name()},
        {"description", p.get_description()}
    };
}

void from_json(const json& j, Profile& p) {
    p.set_name(j.at("name").get<std::string>());
    p.set_description(j.at("description").get<std::string>());
}
