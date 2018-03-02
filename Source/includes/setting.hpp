// Include Guard
#ifndef __SETTING_H_INCLUDED__
#define __SETTING_H_INCLUDED__
//=================================

// Global application settings that don't live anywhere in particular
#include <string>
struct Setting {
    std::string name;
    unsigned int int_value;
    std::string string_value;
};

#endif //__SETTING_H_INCLUDED__
