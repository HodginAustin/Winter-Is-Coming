#include <algorithm>
#include <map>
#include <iostream>
#include <string>

#include "./includes/DataParser.hpp"
#include "./includes/InternalState.hpp"
#include "./includes/StateComposer.hpp"
#include "./includes/Settings.hpp"
#include "./includes/Setting.hpp"

using namespace sqlite_orm;

const std::string DataParser::SETTING_DATABASE_VERSION = "db_version";
const std::string DataParser::NANO_IO_OFFSET = "nano_io_offset";

// logging variables
std::ofstream DataParser::logFile;

// Timing
time_t DataParser::sysTime;
char DataParser::timeBuffer[30];
tm* DataParser::timeInfo;

// Build database schema
inline auto init_storage(const std::string& path)
{
    // Build schema
    return make_storage(path,
        // Settings
        make_table("settings",
            make_column("name",
                        &Setting::name,
                        primary_key()),
            make_column("int_value",
                        &Setting::int_value),
            make_column("string_value",
                        &Setting::string_value)
        ),
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
                        &Zone::get_profile_id,
                        &Zone::set_profile_id),
            foreign_key(&Zone::get_profile_id).references(&Profile::get_id)
        ),
        // LED
        make_table("leds",
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
        make_table("led_states",
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
        make_table("daily_states",
            make_column("id",
                        &DailyState::get_id,
                        &DailyState::set_id,
                        primary_key()),
            make_column("other",
                        &DailyState::other)
        ),
        // Relationship tables //
        // Zone has many LEDs, LEDS are in one zone per profile, but can be in many zones
        make_table("zone_to_led",
            make_column("zone_id", &ZoneToLED::zone_id),
            make_column("led_id", &ZoneToLED::led_id),
            primary_key(&ZoneToLED::zone_id, &ZoneToLED::led_id)
        ),
        // Zone's 7 daily states
        make_table("zone_daily_states",
            make_column("zone_id", &ZoneDOW::zone_id),
            make_column("day_of_week", &ZoneDOW::day_of_week),
            make_column("daily_state_id", &ZoneDOW::daily_state_id),
            primary_key(&ZoneDOW::zone_id,
                        &ZoneDOW::day_of_week)
        ),
        // DailyState has many LEDStates in a map of time<->state pairs
        make_table("daily_state_to_led_state",
            make_column("daily_state_id",
                        &DailyStateToLEDState::daily_state_id),
            make_column("led_state_id",
                        &DailyStateToLEDState::led_state_id),
            make_column("time",
                        &DailyStateToLEDState::time),
            primary_key(&DailyStateToLEDState::daily_state_id,
                        &DailyStateToLEDState::led_state_id,
                        &DailyStateToLEDState::time)
        )
    );
}

// Local database object
using Storage = decltype(init_storage(""));
static std::shared_ptr<Storage> db;


// Initialization
bool DataParser::initialize(bool logEnable)
{
    std::cout << "Initalizing Data Parser... ";

    // Logging
    time(&sysTime);
    timeInfo = localtime(&sysTime);
    strftime(timeBuffer, 30, "%F.dataparser.log", timeInfo);

    if (logEnable) {
        logFile.open(timeBuffer);
        strftime(timeBuffer, 30, "%c", timeInfo);
        logFile << "[" << timeBuffer << "] DataParser transcript started\n";
    }

    // Get database object
    db = std::make_shared<Storage>(init_storage("db.sqlite"));

    // Create DB file
    db->sync_schema();

    // Set database version
    Setting db_version = {SETTING_DATABASE_VERSION, 1, "plantergb v1.0"};
    DataParser::insert(db_version);

    // Load all existing data
    bool getDataSuccess = get_all();

    // Set controller IO offset if not exist
    auto existingOffset = db->get_all<struct Setting>(where(c(&Setting::name) == NANO_IO_OFFSET));
    if (existingOffset.size() == 0)
    {
        Setting controller_offset = {NANO_IO_OFFSET, 2, ""};
        DataParser::insert(controller_offset);
    }

    if (getDataSuccess) {
        std::cout << "done" << std::endl;
    } else {
        std::cout << "failed" << std::endl;
    }
    return getDataSuccess;
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
void DataParser::insert(Setting setting)
{
    db->replace(setting);
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


// DELETE
void DataParser::remove(Profile* p)
{
    // ON CASCADE DELETE
    for (auto zone : p->get_zones()) {
        remove(zone);
    }

    db->remove<Profile>(p->get_id());
}
void DataParser::remove(Zone* z)
{
    // ON CASCADE DELETE
    db->remove_all<ZoneDOW>(where(
        c(&ZoneDOW::zone_id) == z->get_id()
    ));

    db->remove_all<ZoneToLED>(where(
        c(&ZoneToLED::zone_id) == z->get_id()
    ));

    db->remove<Zone>(z->get_id());
}
void DataParser::remove(LED* l)
{
    // ON CASCADE DELETE
    db->remove_all<ZoneToLED>(where(
        c(&ZoneToLED::led_id) == l->get_id()
    ));

    db->remove<LED>(l->get_id());
}
void DataParser::remove(LEDState* ls)
{
    // ON CASCADE DELETE
    db->remove_all<DailyStateToLEDState>(where(
        c(&DailyStateToLEDState::led_state_id) == ls->get_id()
    ));

    db->remove<LEDState>(ls->get_id());
}
void DataParser::remove(DailyState* ds)
{
    // ON CASCADE DELETE
    db->remove_all<ZoneDOW>(where(
        c(&ZoneDOW::daily_state_id) == ds->get_id()
    ));

    db->remove_all<DailyStateToLEDState>(where(
        c(&DailyStateToLEDState::daily_state_id) == ds->get_id()
    ));

    db->remove<DailyState>(ds->get_id());
}
void DataParser::remove(Controller* c)
{
    // ON CASCADE DELETE
    for (auto led : InternalState::get_leds()) {
        if (led->get_controller_id() == c->get_id()) {
            remove(led);
        }
    }

    db->remove<Controller>(c->get_id());
}
void DataParser::remove(Zone* zone, LED* led)
{
    db->remove_all<ZoneToLED>(where(
        c(&ZoneToLED::zone_id) == zone->get_id() and
        c(&ZoneToLED::led_id) == led->get_id()
    ));
}
void DataParser::remove(Zone* zone, int day_of_week)
{
    db->remove_all<ZoneDOW>(where(
        c(&ZoneDOW::zone_id) == zone->get_id() and
        c(&ZoneDOW::day_of_week) == day_of_week
    ));
}
void DataParser::remove(DailyState* dailyState, int time_of_day)
{
    db->remove_all<DailyStateToLEDState>(where(
        c(&DailyStateToLEDState::daily_state_id) == dailyState->get_id() and
        c(&DailyStateToLEDState::time) == time_of_day
    ));
}


// SELECT
Zone* get_zone(std::map<unsigned int, Zone*> tmp, unsigned int id) {
    if (tmp.count(id) > 0) {
        return tmp[id];
    }
    return 0;
}

bool DataParser::get_all()
{
    // Get profiles
    Profile* profile;
    for (auto& p : db->iterate<Profile>()) {
        profile = new Profile(p);
        profile->set_id(p.get_id());
        InternalState::add_profile(profile);
    }
    profile = 0;

    // Get zones
    Zone* zone;
    std::map<unsigned int, Zone*> tmp_zones;
    for (auto& z : db->iterate<Zone>()) {
        zone = new Zone(z);
        zone->set_id(z.get_id());

        tmp_zones.insert(std::make_pair(zone->get_id(), zone));

        profile = InternalState::get_profile(zone->get_profile_id());
        profile->add_zone(zone);
    }

    // Get Controllers
    Controller* controller;
    for (auto& c : db->iterate<Controller>()) {
        controller = new Controller(c);
        controller->set_id(c.get_id());

        InternalState::add_controller(controller);
    }
    
    // Get LEDs
    LED* led;
    for (auto& l : db->iterate<LED>()) {
        led = new LED(l);
        led->set_id(l.get_id());
        
        controller = InternalState::get_controller(led->get_controller_id());
        if (controller) {
            led->set_controller(controller);
            InternalState::add_led(led);
        } else {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB Error: could not find controller (for LED) with id " << l.get_controller_id() << std::endl;
            }
        }
    }
    
    // Get LEDStates
    LEDState* ledState;
    for (auto& ls : db->iterate<LEDState>()) {
        ledState = new LEDState(ls);
        ledState->set_id(ls.get_id());

        InternalState::add_led_state(ledState);
    }

    // Get DailyStates
    DailyState* dailyState;
    for (auto& ds : db->iterate<DailyState>()) {
        dailyState = new DailyState(ds);
        dailyState->set_id(ds.get_id());

        InternalState::add_daily_state(dailyState);
    }

    // Get ZoneToLEDs
    for (auto& ztl : db->iterate<ZoneToLED>()) {
        zone = get_zone(tmp_zones, ztl.zone_id);
        led = InternalState::get_led(ztl.led_id);
        
        if (!zone) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find zone with id " << ztl.zone_id << std::endl;
            }
            continue;
        }
        if (!led) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find led with id " << ztl.led_id << std::endl;
            }
            continue;
        }

        zone->add_led(led);
    }
    
    // Get ZoneDOWs (Day of weeks)
    for (auto& dow : db->iterate<ZoneDOW>()) {
        zone = get_zone(tmp_zones, dow.zone_id);
        dailyState = InternalState::get_daily_state(dow.daily_state_id);
        
        if (!zone) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find zone with id " << dow.zone_id << std::endl;
            }
            continue;
        }
        if (!dailyState) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find daily state with id " << dow.daily_state_id << std::endl;
            }
            continue;
        }

        zone->set_daily_state(dow.day_of_week, dailyState);
    }

    // Get DailyStateToLEDStates
    for (auto& dtl : db->iterate<DailyStateToLEDState>()) {
        dailyState = InternalState::get_daily_state(dtl.daily_state_id);
        ledState = InternalState::get_led_state(dtl.led_state_id);

        if (!dailyState) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find daily state with id " << dtl.daily_state_id << std::endl;
            }
            continue;
        }
        if (!ledState) {
            // Get current time
            time(&sysTime);
            timeInfo=localtime(&sysTime);
            strftime(timeBuffer, 30, "%c", timeInfo);

            if (logFile.is_open()) {
                logFile << "[" << timeBuffer << "] " << "DB relationship Error: could not find led state with id " << dtl.led_state_id << std::endl;
            }
            continue;
        }

        dailyState->add_state(dtl.time, ledState);
    }

    // Get global settings
    for (auto setting : db->iterate<Setting>()) {
        // Add the setting
        Settings::set_setting(setting.name, setting);

        // Currently active profile
        if (setting.name == "current_profile"){
            Profile* profile = InternalState::get_profile(setting.int_value);
            if (profile) {
                InternalState::set_current_profile(profile);
            } else {
                // Get current time
                time(&sysTime);
                timeInfo=localtime(&sysTime);
                strftime(timeBuffer, 30, "%c", timeInfo);

                if (logFile.is_open()) {
                    logFile << "[" << timeBuffer << "] " << "DB Error: could not find current profile with id " << setting.int_value << std::endl;
                }
                continue;
            }
        }
    }

    return true;
}


// Clear
void DataParser::clear()
{
    db->remove_all<Setting>(where(
        c(&Setting::name) != SETTING_DATABASE_VERSION
    ));

    db->remove_all<ZoneDOW>();
    db->remove_all<ZoneToLED>();
    db->remove_all<DailyStateToLEDState>();
    db->remove_all<LED>();
    db->remove_all<LEDState>();
    db->remove_all<Controller>();
    db->remove_all<DailyState>();
    db->remove_all<Zone>();
    db->remove_all<Profile>();
}

// Clean up
void DataParser::clean_up()
{
    clear();
}