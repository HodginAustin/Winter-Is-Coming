var url = "http://localhost:"; // base url
var port = 9080; //default port
var dailyStates = "daily_states";
var profiles = "profiles";
var currentProfile = "current_profile";
var ledStates = "led_states";

function getAllProfiles(...profileNumber){
  var profileURL = url + port + "/" + profiles +"/" + profileNumber[0];
  console.log("profileURL: " + profileURL);
  httpGetAsync(profileURL);
}
