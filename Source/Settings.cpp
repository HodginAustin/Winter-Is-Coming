#include "./includes/Settings.hpp"
#include "./includes/Setting.hpp"

std::unordered_map<std::string, struct Setting> Settings::settings;

struct Setting Settings::get_setting(std::string key)
{
    for (auto s : settings) {
        if (s.first == key){ return s.second; }
    }
    return {};
}

void Settings::set_setting(std::string key, struct Setting s)
{
    // Check if key already exists, update if it does, insert new key if not
    if (settings.count(key) > 0) {
        settings[key] = s;
    } else {
        settings.insert(std::make_pair(key, s));
    }
}
