#include <algorithm>
#include "../includes/Profile.hpp"


// Constructor
Profile::Profile() {}
Profile::Profile(const Profile& p) { copy(p); }


// Copy
void Profile::copy(const Profile& p)
{
    if (!p.get_name().empty()) {
        set_name(p.get_name());
    }
    if (!p.get_description().empty()) {
        set_description(p.get_description());
    }
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

std::vector<Zone*> Profile::get_zones() const
{
    return zones;
}

Zone* Profile::get_zone(unsigned int id) const
{
    for (auto zone : zones) {
        if (zone->get_id() == id){ return zone; }
    }
    return 0;
}

void Profile::delete_zone(Zone* zone)
{
    zones.erase(std::remove(zones.begin(), zones.end(), zone), zones.end());
}


// JSON
void to_json(json& j, const Profile& p) {
    // Build JSON from zone vector
    json zones_j = json::array(); // Empty JSON array []
    std::vector<Zone*> zones = p.get_zones();
    for (auto zone : zones) {
        zones_j.push_back(zone->get_id());
    }

    j = json{
        {"id", p.get_id()},
        {"name", p.get_name()},
        {"description", p.get_description()},
        {"zones", zones_j}
    };
}

void from_json(const json& j, Profile& p) {
    if (j.find("name") != j.end()) {
        p.set_name(j.at("name").get<std::string>());
    }
    if (j.find("description") != j.end()) {
        p.set_description(j.at("description").get<std::string>());
    }
}
