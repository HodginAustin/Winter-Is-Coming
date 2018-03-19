function complete(response) {
  /* Connection settings */
  var conn = require('./global/connection.js');

  $(document).ready(function () {
    var template = "<h1> {{name}} {{description}} </h1>";
    console.log("after template set");
    console.log("response: " + response);
    var profiles = JSON.parse(response);
    console.log("after jsonparse");
    console.log("profiles: " + profiles)
    var html = Mustache.to_html(template, response);
    $('index').html(html);
  });
}

function getAllProfiles() {
  var profileURL = conn.url + conn.port + "/" + conn.profiles;
  var temp;
  console.log("profileURL: " + profileURL);
  httpGetAsync(profileURL, complete);

}

function addProfile() {
  var profileURL = conn.url + conn.port + "/" + conn.profiles + "/add";
  var formData = JSON.stringify($('setProfile').serializeArray());
  httpPostAsync(profileURL, formData);
  // $.ajax({
  //   type: "POST",
  //   url: profileURL,
  //   data: formData,
  //   success: function(){},
  //   dataType: "json",
  //   contentType: header
  // });
  // console.log("profile url: " + profileURL);

}