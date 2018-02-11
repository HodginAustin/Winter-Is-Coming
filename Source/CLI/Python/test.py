import requests


def print_request(r):
    print("    status:" + str(r.status_code))
    print("    text:" + r.text)

def main():
    # URL
    base = "http://localhost:9080"

    # Headers
    h = {'Content-type': 'application/json'}


    print("Get profiles")
    r = requests.get(base + "/profiles", "")
    print_request(r)


    print("Add profile")
    j = {"name": "Test Profile 1", "description": "Potatoes 2"}
    r = requests.post(base + "/profiles/add", json=j, headers=h, timeout=2)
    print_request(r)


    print("Get profiles")
    r = requests.get(base + "/profiles", "")
    print_request(r)


    print("Add Zone to profile 1")
    j = {"name": "Zone 1"}
    r = requests.post(base + "/profiles/1/zones/add", json=j, headers=h, timeout=200)
    print_request(r)

    print("Add Controler")
    j = {"io": 1}
    r = requests.post(base +"/controllers/add", json=j, headers=h)
    print_request(r)

    for i  in range(10):
        print("Add LEDS")
        j = {"strip_idx": i, "controller": 1}
        r = requests.post(base + "/leds/add", json=j, headers=h)
        print_request(r)


    print("Add LEDS to Zone1")
    j = range(1,11)
    r = requests.put(base + "/profiles/1/zones/1/leds/add", json=j, headers=h)
    print_request(r)

    print("add LED states")
    j = {
        "r": 255,
        "g": 255,
        "b": 255,
        "intensity": 100,
        "power": True
            }
    r = requests.post(base + "/led_states/add", json=j, headers=h)
    print_request(r)

    print("add LED2 states")
    j = {
        "r": 255,
        "g": 0,
        "b": 0,
        "intensity": 100,
        "power": True
        }
    r = requests.post(base + "/led_states/add", json=j, headers=h)
    print_request(r)


    print("Add daily state")
    j = [{"time":0, "state":1},{"time":61200, "state":2}]
    r = requests.post(base + "/daily_states/add", json=j, headers=h)
    print_request(r)

    print("Assign daily state to zone")
    r = requests.put(base + "/profiles/1/zones/1/day/0/add/1")
    print_request(r)


    print("Get profiles 1 zones")
    r = requests.get(base + "/profiles/1/zones")
    print_request(r)

    print("Get profiles")
    r = requests.get(base + "/profiles", "")
    print_request(r)

    print("Get daily states")
    r = requests.get(base + "/daily_states", "")
    print_request(r)


    print("Active state")
    r = requests.get(base + "/profiles/1/zones/1/active_state")
    print_request(r)

main()
