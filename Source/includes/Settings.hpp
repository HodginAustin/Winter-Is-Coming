#include <unordered_map>
#include <string>
#include "./Setting.hpp"

class Settings
{
    private:
        static std::unordered_map<std::string, struct Setting> settings;
    public:
        static Setting get_setting(std::string key);

        static void set_setting(std::string key, struct Setting s);
};