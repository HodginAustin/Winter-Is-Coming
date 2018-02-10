// Zone's 7 daily states
struct ZoneDOW {
    int zone_id;
    int ds_sun_id;
    int ds_mon_id;
    int ds_tue_id;
    int ds_wed_id;
    int ds_thu_id;
    int ds_fri_id;
    int ds_sat_id;
};

// Zone has many LEDs, LEDS are in one zone per profile, but can be in many zones
struct ZoneToLED {
    int zone_id;
    int led_id;
};

// DailyState has many LEDStates in a map of time<->state pairs
struct DailyStateToLEDState {
    unsigned int time;
    int daily_state_id;
    int led_state_id;
};
