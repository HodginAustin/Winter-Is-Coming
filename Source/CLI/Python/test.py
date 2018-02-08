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

    print("Get profiles")
    r = requests.get(base + "/profiles", "")
    print_request(r)



    print("Delete Profile 1")
    r = requests.delete(base + "/profiles/1/delete")
    print_request(r)


    print("Get profiles")
    r = requests.get(base + "/profiles", "")
    print_request(r)

main()
