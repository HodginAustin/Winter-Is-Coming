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
    static std::vector<Profile*> profiles;
    static Profile* currentProfile;
public:
    // Initialization
    static void Initialize();

    // Accessors
    static Profile* get_current_profile();

    // CRUD
    static void add_profile(Profile* profile);
    static std::vector<Profile*> get_profiles();
    static void delete_profile(Profile* profile);
};

#endif //__INTERNALSTATE_H_INCLUDED__
