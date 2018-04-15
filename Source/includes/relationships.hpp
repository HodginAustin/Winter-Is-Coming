// Include Guard
#ifndef __RELATIONSHIPS_H_INCLUDED__
#define __RELATIONSHIPS_H_INCLUDED__
//=================================

// Zone's 7 daily states
struct ZoneDOW {
    unsigned int zone_id;
    unsigned int day_of_week;
    unsigned int daily_state_id;
};

// Zone has many LEDs, LEDS are in one zone per profile, but can be in many zones
struct ZoneToLED {
    unsigned int zone_id;
    unsigned int led_id;
};

// DailyState has many LEDStates in a map of time<->state pairs
struct DailyStateToLEDState {
    unsigned int daily_state_id;
    unsigned int time;
    unsigned int led_state_id;
};


#endif //__RELATIONSHIPS_H_INCLUDED__
