var hostname = "localhost:";
var url = "http://" + hostname; // base url
var port = 9080; //default port
var header = "application/json"

// Control service routes
var profiles = "profiles";
var currentProfile = "current_profile";
var dailyStates = "daily_states";
var ledStates = "led_states";
var controllers = "controllers";
var leds = "leds";

module.exports = 
{
    url: url,
    port: port,
    header: header,

    profiles: profiles,
    currentProfile: currentProfile,
    dailyStates: dailyStates,
    ledStates: ledStates,
    controllers: controllers,
    leds: leds
};