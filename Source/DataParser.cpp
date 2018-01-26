#include <algorithm>
#include "./includes/DataParser.hpp"


// Required for static class members
unsigned int DataParser::profile_id;


// Initialization
bool DataParser::initialize()
{
    profile_id = 1;
    return true;
}


// Accessors
unsigned int DataParser::next_profile_id()
{
    unsigned int tmp = profile_id;
    profile_id++;
    return tmp;
}
