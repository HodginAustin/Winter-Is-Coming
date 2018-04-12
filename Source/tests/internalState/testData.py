import requests


def print_request(r):
    print("    status:" + str(r.status_code))
    print("    text:" + r.text)

def main():
    # URL
    base = "http://127.0.0.1:9080"

    # Timeout
    timeout_ms = 5000

    # Headers
    h = {'Content-type': 'application/json'}


    # Add a profile
    print("Add profile")
    j = {"name": "I am profile", "description": "This is my description"}
    r = requests.post(base + "/profiles/add", json=j, headers=h)
    print_request(r)

    # Add a zone to that profile
    print("Add Zone to profile 1")
    j = {"name": "I am Zone"}
    r = requests.post(base + "/profiles/1/zones/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add an LED state
    print("Add LED State 1")
    j = {
        "r": 255,
        "g": 0,
        "b": 0,
        "intensity":80,
        "power":True
    }
    r = requests.post(base + "/led_states/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add another LED state
    print("Add LED State 2")
    j = {
        "r": 0,
        "g": 255,
        "b": 0,
        "intensity":70,
        "power":True
    }
    r = requests.post(base + "/led_states/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add a daily state
    print("Add Daily State")
    j = {
        "timeStateMap":
        [
            {"time":28800,"state":1}, #8 am
            {"time":64800,"state":2}  #6 pm
        ]
    }
    r = requests.post(base + "/daily_states/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add a controller
    print("Add Controller 1")
    j = {
        "io": 0,
        "address":"blahblah",
        "details":""
    }
    r = requests.post(base + "/controllers/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add LEDs to controller 1
    n = 10
    print("Adding {} LEDs".format(n))
    for i in range(n):
        print("Add LED {}".format(i))
        j = {
            "strip_idx":(i+1),
            "controller":1
        }
        r = requests.post(base + "/leds/add", json=j, headers=h, timeout=timeout_ms)
        print_request(r)

    # Add LEDs to zone 1
    print("Add LEDs to Zone 1")
    j = range(1,n+1)
    r = requests.put(base + "/profiles/1/zones/1/leds/add", json=j, headers=h, timeout=timeout_ms)
    print_request(r)

    # Add DailyState 1 to Zone 1 on day 2 (Tuesday)
    print("Add DailyState to Zone (Tuesday)")
    r = requests.put(base + "/profiles/1/zones/1/day/2/add/1", timeout=timeout_ms)
    print_request(r)

    # Get active state for zone 1 (Varies based on time)
    print("Get active state for zone 1")
    print("If currently after Tuesday 6pm, will return LEDState ID 2")
    print("If before Tuesday 6pm and after Tuesday 8am, will return LEDState ID 1")
    r = requests.get(base + "/profiles/1/zones/1/active_state", timeout=timeout_ms)
    print_request(r)

main()
