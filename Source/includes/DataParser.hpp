// Include Guard
#ifndef __DATA_PARSER_H_INCLUDED__
#define __DATA_PARSER_H_INCLUDED__
//=================================


#include <vector>
#include <fstream>
#include <string>

#include "./Profile.hpp"
#include "./Zone.hpp"
#include "./LED.hpp"
#include "./Controller.hpp"
#include "./LEDState.hpp"
#include "./DailyState.hpp"

#include "./relationships.hpp"
#include "./Setting.hpp"

#include "./sqlite_orm.h"

using namespace sqlite_orm;

class DataParser
{
private:
    // Timing objects
    static time_t sysTime;
    static char timeBuffer[30];
    static tm* timeInfo;

    // Logging
    static std::ofstream logFile;

public:
    // Const settings
    static const std::string SETTING_DATABASE_VERSION;
    static const std::string NANO_IO_OFFSET;

    // Initialization
    static bool initialize(bool logEnable);

    // INSERT
    static unsigned int insert(Profile*);
    static unsigned int insert(Zone*);
    static unsigned int insert(LED*);
    static unsigned int insert(LEDState*);
    static unsigned int insert(DailyState*);
    static unsigned int insert(Controller*);
    static void insert(ZoneDOW);
    static void insert(ZoneToLED);
    static void insert(DailyStateToLEDState);
    static void insert(Setting setting);
    
    // UPDATE
    static void update(Profile*);
    static void update(Zone*);
    static void update(LED*);
    static void update(LEDState*);
    static void update(DailyState*);
    static void update(Controller*);

    // SELECT
    static Profile* selectProfile(unsigned int);
    static Zone* selectZone(unsigned int);
    static LED* selectLED(unsigned int);
    static LEDState* selectLEDState(unsigned int);
    static DailyState* selectDailyState(unsigned int);
    static Controller* selectController(unsigned int);
    static Setting selectSetting(std::string);
    static ZoneDOW selectZoneDOW(unsigned int);
    static ZoneToLED selectZoneToLED(unsigned int);
    static DailyStateToLEDState selectDailyStateToLEDState(unsigned int);
    static bool get_all();

    // DELETE
    static void remove(Profile*);
    static void remove(Zone*);
    static void remove(LED*);
    static void remove(LEDState*);
    static void remove(DailyState*);
    static void remove(Controller*);
    static void remove(Zone*, LED*);
    static void remove(Zone*, int);
    static void remove(DailyState*, int);

    static void clear();
};

#endif //__DATA_PARSER_H_INCLUDED__
