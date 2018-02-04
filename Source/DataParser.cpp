#include <algorithm>
#include "./includes/DataParser.hpp"


using namespace sqlite_orm;


// Required for static class members
unsigned int DataParser::profile_id;
unsigned int DataParser::zone_id;
unsigned int DataParser::schedule_id;
unsigned int DataParser::led_id;
unsigned int DataParser::led_state_id;
unsigned int DataParser::daily_state_id;
unsigned int DataParser::controller_id;


// Build database schema
inline auto init_storage(const std::string& path)
{
    // Build schema
    return make_storage(path,
        make_table("zones",
            make_column("id",
                        &Zone::id,
                        primary_key()),
            make_column("name",
                        &Zone::name)));
}

typedef decltype(init_storage("")) Storage;
static std::shared_ptr<Storage> db;


// Initialization
bool DataParser::initialize()
{
    profile_id = zone_id = schedule_id = led_id = led_state_id = daily_state_id = controller_id = 1;

    // Get database object
    db = std::make_shared<Storage>(init_storage("db.sqlite"));

    // Create DB file
    db->sync_schema();
    
    return true;
}


// CRUD operations
unsigned int DataParser::insert(Zone* z)
{
    return 0;
    //return db->insert(*z);
}
unsigned int DataParser::insert(Profile* p)
{
    return 0;
    //return db->insert(*p);
}


// ID Accessors
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
