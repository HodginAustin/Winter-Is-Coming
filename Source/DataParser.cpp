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
        // Profiles
        make_table("profiles",
            make_column("id",
                        &Profile::get_id,
                        &Profile::set_id,
                        primary_key()),
            make_column("name",
                        &Profile::get_name,
                        &Profile::set_name),
            make_column("description",
                        &Profile::get_description,
                        &Profile::set_description)
        ),
        // Schedule
        make_table("schedules",
            make_column("id",
                        &Schedule::get_id,
                        &Schedule::set_id,
                        primary_key())
        ),
        // Zones
        make_table("zones",
            make_column("id",
                        &Zone::get_id,
                        &Zone::set_id,
                        primary_key()),
            make_column("name",
                        &Zone::get_name,
                        &Zone::set_name),
            make_column("profile_id",
                        &Zone::profile_id),
            foreign_key(&Zone::profile_id).references(&Profile::id),
            make_column("schedule_id",
                        &Zone::schedule_id),
            foreign_key(&Zone::schedule_id).references(&Schedule::get_id)
        ),
        // LED
        make_table("led",
            make_column("id",
                        &LED::get_id,
                        &LED::set_id,
                        primary_key()),
            make_column("strip_idx",
                        &LED::get_strip_idx,
                        &LED::set_strip_idx),
            make_column("controller_id",
                        &LED::controller_id),
            foreign_key(&LED::controller_id).references(&Controller::get_id)
        ),
        // Controller
        make_table("controllers",
            make_column("id",
                        &Controller::get_id,
                        &Controller::set_id,
                        primary_key()),
            make_column("io",
                        &Controller::get_io,
                        &Controller::set_io),
            make_column("address",
                        &Controller::get_address,
                        &Controller::set_address),
            make_column("details",
                        &Controller::get_details,
                        &Controller::set_details)
        ),
        // LED State
        make_table("led_state",
            make_column("id",
                        &LEDState::get_id,
                        &LEDState::set_id,
                        primary_key()),
            make_column("r",
                        &LEDState::get_r,
                        &LEDState::set_r),
            make_column("g",
                        &LEDState::get_g,
                        &LEDState::set_g),
            make_column("b",
                        &LEDState::get_b,
                        &LEDState::set_b),
            make_column("intensity",
                        &LEDState::get_intensity,
                        &LEDState::set_intensity),
            make_column("power",
                        &LEDState::get_power,
                        &LEDState::set_power)
        ),
        // Daily State
        make_table("daily_state",
            make_column("id",
                        &DailyState::get_id,
                        &DailyState::set_id,
                        primary_key())
        )

        // Relationship tables
    );
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
