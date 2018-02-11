#include <algorithm>
#include <map>
#include <iostream>

#include "./includes/DataParser.hpp"
#include "./includes/InternalState.hpp"

using namespace sqlite_orm;


// Build database schema
inline auto init_storage(const std::string& path)
{
    // Build schema
    return make_storage(path,
        // Settings
        //make_table("settings",
        //    make_column("current_profile",
        //                &InternalState::currentProfileId)
        //),
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
            foreign_key(&Zone::profile_id).references(&Profile::get_id)
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
                        &LED::get_controller_id,
                        &LED::set_controller_id),
            foreign_key(&LED::get_controller_id).references(&Controller::get_id)
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
        ),
        // Relationship tables //
        // Zone's 7 daily states
        make_table("zone_to_led",
            make_column("zone_id", &ZoneToLED::zone_id),
            make_column("led_id", &ZoneToLED::led_id),
            primary_key(&ZoneToLED::zone_id, &ZoneToLED::led_id)
        ),
        // Zone has many LEDs, LEDS are in one zone per profile, but can be in many zones
        make_table("zone_daily_states",
            make_column("zone_id", &ZoneDOW::zone_id),
            make_column("ds_sun_id", &ZoneDOW::ds_sun_id),
            make_column("ds_mon_id", &ZoneDOW::ds_mon_id),
            make_column("ds_tue_id", &ZoneDOW::ds_tue_id),
            make_column("ds_wed_id", &ZoneDOW::ds_wed_id),
            make_column("ds_thu_id", &ZoneDOW::ds_thu_id),
            make_column("ds_fri_id", &ZoneDOW::ds_fri_id),
            make_column("ds_sat_id", &ZoneDOW::ds_sat_id)
        ),
        // DailyState has many LEDStates in a map of time<->state pairs
        make_table("daily_state_to_led_state",
            make_column("time",
                        &DailyStateToLEDState::time,
                        primary_key()),
            make_column("daily_state_id",
                        &DailyStateToLEDState::daily_state_id),
            make_column("led_state_id",
                        &DailyStateToLEDState::led_state_id)
        )
    );
}

using Storage = decltype(init_storage(""));
static std::shared_ptr<Storage> db;


// Initialization
bool DataParser::initialize()
{
    // Get database object
    db = std::make_shared<Storage>(init_storage("db.sqlite"));

    // Create DB file
    db->sync_schema();

    // Load all existing data
    get_all();
    
    return true;
}


// INSERT
unsigned int DataParser::insert(Profile* p)
{
    unsigned int id = db->insert(*p);
    p->set_id(id);
    return id;
}
unsigned int DataParser::insert(Zone* z)
{
    unsigned int id = db->insert(*z);
    z->set_id(id);
    return id;
}
unsigned int DataParser::insert(LED* l)
{
    unsigned int id = db->insert(*l);
    l->set_id(id);
    return id;
}
unsigned int DataParser::insert(LEDState* ls)
{
    unsigned int id = db->insert(*ls);
    ls->set_id(id);
    return id;
}
unsigned int DataParser::insert(DailyState* ds)
{
    unsigned int id = db->insert(*ds);
    ds->set_id(id);
    return id;
}
unsigned int DataParser::insert(Controller* c)
{
    unsigned int id = db->insert(*c);
    c->set_id(id);
    return id;
}
void DataParser::insert(ZoneDOW dow)
{
    db->replace(dow);
}
void DataParser::insert(ZoneToLED ztl)
{
    db->replace(ztl);
}
void DataParser::insert(DailyStateToLEDState dsl)
{
    db->replace(dsl);
}


// UPDATE
void DataParser::update(Profile* p)
{
    db->update(*p);
}
void DataParser::update(Zone* z)
{
    db->update(*z);
}
void DataParser::update(LED* l)
{
    db->update(*l);
}
void DataParser::update(LEDState* ls)
{
    db->update(*ls);
}
void DataParser::update(DailyState* ds)
{
    db->update(*ds);
}
void DataParser::update(Controller* c)
{
    db->update(*c);
}


// SELECT
Zone* get_zone(std::map<unsigned int, Zone*> tmp, unsigned int id) {
    if (tmp.count(id) > 0) {
        return tmp[id];
    }
    return 0;
}
void DataParser::get_all()
{
    // Get profiles
    Profile* profile;
    for (auto& p : db->iterate<Profile>()) {
        profile = new Profile(p);
        profile->set_id(p.get_id());
        InternalState::add_profile(profile);
    }

    // Get zones
    Zone* zone;
    std::map<unsigned int, Zone*> tmp_zones;
    for (auto& z : db->iterate<Zone>()) {
        zone = new Zone(z);

        tmp_zones.insert(std::make_pair(zone->get_id(), zone));

        profile = InternalState::get_profile(zone->profile_id);
        profile->add_zone(zone);
    }

    // Get Controllers
    Controller* controller;
    for (auto& c : db->iterate<Controller>()) {
        controller = new Controller(c);
        InternalState::add_controller(controller);
    }
    
    // Get LEDs
    LED* led;
    for (auto& l : db->iterate<LED>()) {
        led = new LED(l);
        if (led) {
            controller = InternalState::get_controller(led->get_controller_id());
            led->set_controller(controller);
        } else {
            std::cout << "DB Error: could not find led with id " << l.get_id() << std::endl;
        }
    }

    // Get LEDStates
    LEDState* ledState;
    for (auto& ls : db->iterate<LEDState>()) {
        ledState = new LEDState(ls);
        InternalState::add_led_state(ledState);
    }

    // Get DailyStates
    DailyState* dailyState;
    for (auto& ds : db->iterate<DailyState>()) {
        dailyState = new DailyState(ds);
        InternalState::add_daily_state(dailyState);
    }

    // Get ZoneDOWs (Day of weeks)
    std::array<int, 7> days;
    for (auto& dow : db->iterate<ZoneDOW>()) {
        zone = get_zone(tmp_zones, dow.zone_id);
        
        if (zone) {
            days = dow.get_days();
            for (std::size_t i = 0, max = days.size(); i != max; ++i) {
                dailyState = InternalState::get_daily_state(days.at(i));
                zone->set_daily_state(i, dailyState);
            }
        } else {
            std::cout << "DB Error: could not find zone with id " << dow.zone_id << std::endl;
        }
    }

    // Get ZoneToLEDs
    for (auto& ztl : db->iterate<ZoneToLED>()) {
        zone = get_zone(tmp_zones, ztl.zone_id);
        led = InternalState::get_led(ztl.led_id);
        
        if (!zone) {
            std::cout << "DB Error: could not find zone with id " << ztl.zone_id << std::endl;
            continue;
        }
        if (!led) {
            std::cout << "DB Error: could not find led with id " << ztl.led_id << std::endl;
            continue;
        }

        zone->add_led(led);
    }

    // Get DailyStateToLEDStates
    for (auto& dtl : db->iterate<DailyStateToLEDState>()) {
        dailyState = InternalState::get_daily_state(dtl.daily_state_id);
        ledState = InternalState::get_led_state(dtl.led_state_id);

        if (!dailyState) {
            std::cout << "DB Error: could not find daily state with id " << dtl.daily_state_id << std::endl;
            continue;
        }
        if (!ledState) {
            std::cout << "DB Error: could not find led state with id " << dtl.led_state_id << std::endl;
            continue;
        }

        dailyState->add_state(dtl.time, ledState);
    }
}


// DELETE
void DataParser::remove(Profile* p)
{
    db->remove<Profile>(p->get_id());
}
void DataParser::remove(Zone* z)
{
    db->remove<Profile>(z->get_id());
}
void DataParser::remove(LED* l)
{
    db->remove<Profile>(l->get_id());
}
void DataParser::remove(LEDState* ls)
{
    db->remove<Profile>(ls->get_id());
}
void DataParser::remove(DailyState* ds)
{
    db->remove<Profile>(ds->get_id());
}
void DataParser::remove(Controller* c)
{
    db->remove<Profile>(c->get_id());
}


// Clear
void DataParser::clear()
{
    // Entities
    db->remove_all<Profile>();
    db->remove_all<Zone>();
    db->remove_all<LED>();
    db->remove_all<LEDState>();
    db->remove_all<DailyState>();
    db->remove_all<Controller>();
    // Relationships
    //db->remove_all<ZoneDOW>();
    //db->remove_all<ZoneToLED>();
    //db->remove_all<DailyStateToLEDState>();    
}
