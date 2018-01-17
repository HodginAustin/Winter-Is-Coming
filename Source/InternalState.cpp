#include <algorithm>
#include "./includes/InternalState.hpp"


// Initialization
void InternalState::Initialize()
{
    currentProfile = 0;
}


// Accessors
Profile* InternalState::get_current_profile()
{
    return currentProfile;
}


// CRUD
void InternalState::add_profile(Profile* profile)
{
    profiles.push_back(profile);
}

std::vector<Profile*> InternalState::get_profiles()
{
    return profiles;
}

void InternalState::delete_profile(Profile* profile)
{
    profiles.erase(std::remove(profiles.begin(), profiles.end(), profile), profiles.end());
}
