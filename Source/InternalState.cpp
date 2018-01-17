#include <algorithm>
#include "./includes/InternalState.hpp"

// Required for static class members
std::vector<Profile*> InternalState::profiles;
Profile* InternalState::currentProfile;

// Initialization
void InternalState::Initialize()
{
    InternalState::currentProfile = 0;
}


// Accessors
Profile* InternalState::get_current_profile()
{
    return InternalState::currentProfile;
}


// CRUD
void InternalState::add_profile(Profile* profile)
{
    InternalState::profiles.push_back(profile);
}

std::vector<Profile*> InternalState::get_profiles()
{
    return InternalState::profiles;
}

void InternalState::delete_profile(Profile* profile)
{
    InternalState::profiles.erase(
        std::remove(InternalState::profiles.begin(), InternalState::profiles.end(), profile),
        InternalState::profiles.end());
}
