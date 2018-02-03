#include <algorithm>
#include "./includes/DataParser.hpp"

#include "./includes/sqlite_orm.hpp"


using namespace sqlite_orm;


// Required for static class members
unsigned int DataParser::profile_id;
unsigned int DataParser::zone_id;
unsigned int DataParser::schedule_id;
unsigned int DataParser::led_id;
unsigned int DataParser::led_state_id;
unsigned int DataParser::daily_state_id;
unsigned int DataParser::controller_id;


// Initialization
bool DataParser::initialize()
{
    profile_id = zone_id = schedule_id = led_id = led_state_id = daily_state_id = controller_id = 1;
    return true;
}

// sudo apt-get install sqlite3
storage_t<Ts...> DataParser::make_storage()
{
    storage = make_storage("db.sqlite",
        make_table("ZONE",
            make_column("ID",
                &Zone::id,
                primary_key()
            ),
        )
    );
}


// Accessors
unsigned int DataParser::next_profile_id()
{
    unsigned int tmp = profile_id;
    profile_id++;
    return tmp;
}

unsigned int DataParser::next_zone_id()
{
    unsigned int tmp = zone_id;
    zone_id++;
    return tmp;
}

unsigned int DataParser::next_schedule_id()
{
    unsigned int tmp = schedule_id;
    schedule_id++;
    return tmp;
}

unsigned int DataParser::next_led_id()
{
    unsigned int tmp = led_id;
    led_id++;
    return tmp;
}

unsigned int DataParser::next_led_state_id()
{
    unsigned int tmp = led_state_id;
    led_state_id++;
    return tmp;
}

unsigned int DataParser::next_daily_state_id()
{
    unsigned int tmp = daily_state_id;
    daily_state_id++;
    return tmp;
}

unsigned int DataParser::next_controller_id()
{
    unsigned int tmp = controller_id;
    controller_id++;
    return tmp;
}
