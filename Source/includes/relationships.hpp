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

    std::array<int, 7> get_days() {
        return {ds_sun_id, ds_mon_id, ds_tue_id, ds_wed_id, ds_thu_id, ds_fri_id, ds_sat_id};
    }
};

// Zone has many LEDs, LEDS are in one zone per profile, but can be in many zones
struct ZoneToLED {
    int zone_id;
    int led_id;
};

// DailyState has many LEDStates in a map of time<->state pairs
struct DailyStateToLEDState {
    int daily_state_id;
    unsigned int time;
    int led_state_id;
};
