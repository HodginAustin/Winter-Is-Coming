#include "../asserting.h"
#include "../../includes/StateComposer.hpp"


bool test_state()
{

    bool condTrue = true;
    bool condFalse = false;

    // Initialize
    condTrue &= assertTrue(StateComposer::initialize(true));

    condTrue &= assertTrue(StateComposer::get_composer_state() == 'c');

    StateComposer::set_composer_state('p');

    condTrue &= assertTrue(StateComposer::get_composer_state() == 'p');

    StateComposer::set_composer_state('c');

    condTrue &= assertTrue(StateComposer::get_composer_state() == 'c');

    StateComposer::set_composer_state('k');     // Set to known bad value

    condFalse &= assertTrue(StateComposer::get_composer_state() == 'k');
    condTrue &= assertTrue(StateComposer::get_composer_state() == 'c');     // Should still be 'c'

    StateComposer::set_composer_state(257);     // Out of bounds (overflow)

    condFalse &= assertTrue(StateComposer::get_composer_state() == 257);
    condTrue &= assertTrue(StateComposer::get_composer_state() == 'c');     // Should still be 'c'

    StateComposer::set_composer_state(0);       // Kills state composer thread

    condFalse &= assertTrue(StateComposer::get_composer_state() == 'c');    // Should be dead, no 'c'
    condTrue &= assertTrue(StateComposer::get_composer_state() == 0);

    return condTrue && !condFalse;
}