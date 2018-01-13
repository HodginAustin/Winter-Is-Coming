// Include Guard
#ifndef __INTERNALSTATE_H_INCLUDED__
#define __INTERNALSTATE_H_INCLUDED__
//=================================


#include <vector>
#include "./Profile.hpp"
#include "./Zone.hpp"

class InternalState
{
private:
    std::vector<Profile*> profiles;
    Profile* currentProfile;
public:
    // Constructor
    InternalState();
};

#endif //__INTERNALSTATE_H_INCLUDED__
