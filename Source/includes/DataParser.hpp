// Include Guard
#ifndef __DATA_PARSER_H_INCLUDED__
#define __DATA_PARSER_H_INCLUDED__
//=================================


#include <vector>
#include "./Profile.hpp"
#include "./Zone.hpp"
#include "./LED.hpp"
#include "./Controller.hpp"
#include "./LEDState.hpp"
#include "./DailyState.hpp"

class DataParser
{
private:
    static unsigned int profile_id;
public:
    // Initialization
    static bool initialize();

    // Accessors
    static unsigned int next_profile_id();
};

#endif //__DATA_PARSER_H_INCLUDED__
