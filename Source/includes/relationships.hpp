// Zone's 7 daily states
struct ZoneDOW {
    unsigned int zone_id;
    unsigned int ds_sun_id;
    unsigned int ds_mon_id;
    unsigned int ds_tue_id;
    unsigned int ds_wed_id;
    unsigned int ds_thu_id;
    unsigned int ds_fri_id;
    unsigned int ds_sat_id;

    std::array<unsigned int, 7> get_days() {
        return {ds_sun_id, ds_mon_id, ds_tue_id, ds_wed_id, ds_thu_id, ds_fri_id, ds_sat_id};
    }
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
