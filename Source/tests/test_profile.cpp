#include <vector>

#include "./asserting.h"
#include "../includes/Profile.hpp"

bool test_profile()
{
    Profile* p = new Profile(9);
    Zone* z1 = new Zone(1);
    Zone* z2 = new Zone(2);
    Zone* z3 = new Zone(3);

    bool condTrue = true;
    bool condFalse = false;
    
    // ID
    condTrue &= assertTrue(p->get_id() == 9);

    // zones vector empty initially
    condTrue &= assertTrue(p->get_zones().size()==0);

    // Add zone to profile
    p->add_zone(z1);
    condTrue &= assertTrue(p->get_zones().size()==1);
    condTrue &= assertTrue(p->get_zones()[0]==z1);


    // Delete non-existent zone and confirm that count does not change
    p->delete_zone(z3);
    condTrue &= assertTrue(p->get_zones().size()==1);

    // Add second zone to profile
    p->add_zone(z2);
    condTrue &= assertTrue(p->get_zones()[1]==z2);
    condTrue &= assertTrue(p->get_zones().size()==2);

    // Delete existing zones
    p->delete_zone(z1);
    condTrue &= assertTrue(p->get_zones().size()==1);
    p->delete_zone(z2);
    condTrue &= assertTrue(p->get_zones().size()==0);


    //condFalse |= assertFalse();

    return condTrue && !condFalse;
}