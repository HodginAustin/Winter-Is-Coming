/***********************
* Name: httpGetAsync
* Parameters: url for get request
* Returns: response text
* Notes: Http get request using async XMLHttpRequest
* References: https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Synchronous_and_Asynchronous_Requests
***********************/
function httpGetAsync(url){
  var xhr = new XMLHttpRequest();
  xhr.open(GET, url, true);
  xhr.onload = function(e){
    if(xhr.readyState == 4 && xhr.status == 200){
      console.log(xhr.responseText); // test
      return xhr.responseText;
    }
    else{
      console.log("ERROR LOADING RESPONSE: " + xhr.statusText);
    }
  };
  xhr.onerror = function(e){
    console.error(xhr.statusText)
  };
  xhr.send(null);
}


/*************************************************
* Name: httpPostAsync
* Parameters: url for get request, data to send
* Returns: response text
* Notes: Http post request using async XMLHttpRequest
***************************************************/
function httpPostAsync(url, data){
  var xhr = new XMLHttpRequest();
  xhr.open(POST, url, true);
  xhr.onload = function(e){
    if(xhr.readyState == 4 && xhr.status == 200){
      console.log(xhr.responseText); // test
      return xhr.responseText;
    }
    else{
      console.log("ERROR LOADING RESPONSE: " + xhr.statusText);
    }
  };
  xhr.onerror = function(e){
    console.error(xhr.statusText)
  };
  xhr.send(data);
}
