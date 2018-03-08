/***********************
* Name: httpGetAsync
* Parameters: url for get request
* Returns: response text
* Notes: Http get request using async XMLHttpRequest
* References: https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Synchronous_and_Asynchronous_Requests
***********************/
function httpGetAsync(url, complete){
  var getResponse;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function(){
    console.log("xhr.readystate:" + xhr.readyState);
    console.log("xhr.status: " + xhr.status);
    if(xhr.readyState == 4 && xhr.status == 200){
      console.log("Before Complete is called");
      complete(xhr.responseText);
    }
  };
  xhr.open('GET', url, true); // true for async, false for sync (sync is depricated in some browsers) defualt: true
  xhr.send();
}


/*************************************************
* Name: httpPostAsync
* Parameters: url for post request, data to send
* Returns: response text
* Notes: Http post request using async XMLHttpRequest
***************************************************/
function httpPostAsync(url, data){
  console.log("data: " + data);
  temp = data
  var getResponse;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function(){
    console.log("xhr.readystate:" + xhr.readyState);
    console.log("xhr.status: " + xhr.status);
    if(xhr.readyState == 4 && xhr.status == 200){
    }
  };
  xhr.open('POST', url, true); // true for async, false for sync (sync is depricated in some browsers) defualt: true
  xhr.send(data);
}


function test(){
  console.log("temp: " + temp);
}

/*************************************************
* Name: httpPuttAsync
* Parameters: url for post request, data to send
* Returns: response text
* Notes: Http post request using async XMLHttpRequest
***************************************************/
function httpPutAsync(url, data){
  var getResponse;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function(){
    console.log("xhr.readystate:" + xhr.readyState);
    console.log("xhr.status: " + xhr.status);
    if(xhr.readyState == 4 && xhr.status == 200){
      console.log("Before Complete is called");
      complete(xhr.responseText);
    }
  };
  xhr.open('PUT', url, true); // true for async, false for sync (sync is depricated in some browsers) defualt: true
  xhr.send(data);
}
