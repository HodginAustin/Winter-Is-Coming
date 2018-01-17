#include <vector>

#include "./asserting.h"
#include "../includes/InternalState.hpp"

bool test_internal_state_manager()
{
    InternalState::Initialize();

    Profile* p1 = new Profile(1);
    Profile* p2 = new Profile(2);
    Profile* p3 = new Profile(3);

    bool condTrue = true;
    bool condFalse = false;

    // profiles vector empty initially
    condTrue &= assertTrue(InternalState::get_profiles().size()==0);

    // Add profile to internal state
    InternalState::add_profile(p1);
    condTrue &= assertTrue(InternalState::get_profiles().size()==1);
    condTrue &= assertTrue(InternalState::get_profiles()[0]==p1);


    // Delete non-existent profile and confirm that count does not change
    InternalState::delete_profile(p3);
    condTrue &= assertTrue(InternalState::get_profiles().size()==1);

    // Add second profile to internal state
    InternalState::add_profile(p2);
    condTrue &= assertTrue(InternalState::get_profiles()[1]==p2);
    condTrue &= assertTrue(InternalState::get_profiles().size()==2);

    // Delete existing profiles
    InternalState::delete_profile(p1);
    condTrue &= assertTrue(InternalState::get_profiles().size()==1);
    InternalState::delete_profile(p2);
    condTrue &= assertTrue(InternalState::get_profiles().size()==0);


    return condTrue && !condFalse;
}