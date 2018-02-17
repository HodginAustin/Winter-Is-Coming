from datetime import datetime
import requests
import json
import os
import sys


def get_time():
    now = datetime.now()
    return (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()


def get_dow():
    d = datetime.today().weekday() + 1
    return d if d <= 6 else 0


def print_request(r):
    print("    status:" + str(r.status_code))
    print("    text:" + r.text)


def initialize(url, header):

    print("Adding Default Profile")
    j = {"name": "Default Profile", "description": "Default Profile"}
    r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=200)

    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Set current profile")
    j = requests.post(url + "/current_profile/1","", timeout=200)
    if(str(r.status_code) == "200"):
        print("Set current profile successful")

    print("Add Zone to  Default profile 1")
    j = {"name": "Zone 1"}
    r = requests.post(url + "/profiles/1/zones/add", json=j, headers=header, timeout=200)

    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Add 1 Controler To Default Controller")
    j = {"io": 1}
    r = requests.post(url +"/controllers/add", json=j, headers=header)

    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Creating 10 LED's")

    for i in range(30):
        print("Add LEDS")
        j = {"strip_idx": i, "controller": 1}
        r = requests.post(url + "/leds/add", json=j, headers=header)
        if(str(r.status_code) == "200"):
            print("Add Successful for %i" % i)

    print("Adding  LEDS to Zone1")
    j = range(1,31)
    r = requests.put(url + "/profiles/1/zones/1/leds/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Adding White LED state 100 intensity")
    j = {
        "r": 255,
        "g": 255,
        "b": 255,
        "intensity": 100,
        "power": True
        }
    r = requests.post(url + "/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Add Blank Daily State To Zone Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Add Daily State Time Mapping")
    j = [{"time":get_time(), "state":1}]
    r = requests.put(url + "/daily_states/1/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add successful")

    print("Assign daily state to zone For Sunday")
    r = requests.put(url + "/profiles/1/zones/1/day/0/add/1")
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Current Active state")
    r = requests.get(url + "/profiles/1/zones/1/active_state")
    print_request(r)

    print("Initialization Complete")


def getProfiles(url, header):

    r = requests.get(url + "/profiles", "")
    if (str(r.status_code) == "200"):
        return r.json()


def shutdown(url, header):
    os.system('cls' if os.name == 'nt' else 'clear')
    print("Shutting down PlanteRGB Service.....")
    r = requests.get(url + "/shutdown")
    if str(r.status_code) == "200":
        print("Shut down Successful!")


def configure_Zones(url, header, zoneJson):
    #os.system('cls' if os.name == 'nt' else 'clear')
    print("|------------------------Zone Menu------------------------|")
    option = input("Which Zone would you like to configure (1-4): ")

    return zoneJson


def configure_Schedule(url, header):
    os.system('cls' if os.name == 'nt' else 'clear')


def configure_profiles(url,header,profilesJoson):

    os.system('cls' if os.name == 'nt' else 'clear')
    r = getProfiles(url, header)

    print(r)

    option = 0
    numProfiles = 0
    results = []

    for val in r:
        numProfiles = numProfiles + 1

    if numProfiles > 0:
        while option != 3:
            print("|---------------------------Profile Menu---------------------------|")
            option = input("Add New Profile(1) | Edit Existing Profile (2) | Exit Profile(3) ")

            if option == 1:
                name = raw_input("Enter in profile name: ")
                description = raw_input("Enter description for new profile: ")
                profilesJoson = {"name" : str(name), "description": str(description)}
            elif option == 2:
                print("Will add later!")
                numEdit = input("Which Profile would you like to edit? (1-" + str(numProfiles) + "): ")
    else:
        print("Please run the initialize option to set default profiles")

    return profilesJoson


def sendToServer(url, zoneJson, scheduleJson, profilesJoson):

    #check to see if anything has changed in zones
    if zoneJson:
        return zoneJson
    #check to see if anything has changed in schedule
    if scheduleJson:
        return scheduleJson

    #checking to see if anything has changed in profiles
    if profilesJoson:
        return profilesJoson


def createDemo1(url, header):

    seconds_since_midnight = get_time()

    oneHourLater = seconds_since_midnight + 3600

    time = seconds_since_midnight

    #will be state 2
    print("Adding Red Daily state 100 intensity")
    j = {
        "r": 255,
        "g": 0,
        "b": 0,
        "intensity": 100,
        "power": True
        }
    r = requests.post(url + "/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    #Will be state 3
    print("Adding Green Daily state 100 intensity")
    j = {
        "r": 0,
        "g": 255,
        "b": 0,
        "intensity": 100,
        "power": True
        }
    r = requests.post(url + "/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")
    #will be state 4
    print("Adding Blue Daily state 100 intensity")
    j = {
        "r": 0,
        "g": 0,
        "b": 255,
        "intensity": 100,
        "power": True
        }
    r = requests.post(url + "/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")



    while time < oneHourLater:
        j = []
        print("Adding Red")
        j.append({"time": time, "state":2})
        time = time + 10
        print("Adding Green")
        j.append({"time": time, "state":3})
        time = time +10
        print("Adding Blue")
        j.append({"time": time, "state":4})
        time = time + 10
        r = requests.put(url + "/daily_states/1/led_states/add", json=j, headers=header)
        if(str(r.status_code) == "200"):
            print("Add Successful")

    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/1/zones/1/day/%i/add/1" % get_dow())
    print_request(r)


def createDemo2(url, header):
    seconds_since_midnight = get_time()

    print("Creating Profile 2")

    print("Adding Profile2")
    j = {"name": "Profile2", "description": "Profile2"}
    r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=200)

    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Set current profile")
    j = requests.post(url + "/current_profile/2","", timeout=200)
    if(str(r.status_code) == "200"):
        print("Set current profile successful")

    print("Add Zone 1 to profile2")
    j = {"name": "Zone 1"}
    r = requests.post(url + "/profiles/2/zones/add", json=j, headers=header, timeout=200)

    if(str(r.status_code) == "200"):
        print("Add Successful")

	print("Adding  LEDS to Zone1")
    j = range(1,15)
    r = requests.put(url + "/profiles/2/zones/2/leds/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Add Blank Daily State To Zone Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    j = []
    print("Adding Red to Zone 1")
    j.append({"time": seconds_since_midnight, "state":2})
    r = requests.put(url + "/daily_states/2/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/2/zones/2/day/%i/add/2" % get_dow())
    print_request(r)

    j = []

    seconds_since_midnight += 10

    print("Add Zone 2 to  profile2")
    j = {"name": "Zone 2"}
    r = requests.post(url + "/profiles/2/zones/add", json=j, headers=header, timeout=200)

    print("Adding  LEDS to Zone2")
    j = range(15,31)
    r = requests.put(url + "/profiles/2/zones/3/leds/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Add Blank Daily State To Zone 2 Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    #state 5
    print("Adding teal Daily state 100 intensity")
    j = {
        "r": 0,
        "g": 128,
        "b": 128,
        "intensity": 100,
        "power": True
        }

    r = requests.post(url + "/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    j = []
    j.append({"time": seconds_since_midnight, "state":5})

    r = requests.put(url + "/daily_states/3/led_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/2/zones/3/day/%i/add/3" % get_dow())
    if(str(r.status_code) == "200"):
        print("Add Successful")


def main():
    #URL
    port = sys.argv[1] if len(sys.argv)>1 else "9080"
    url = "http://localhost:" + port
    # Headers
    header = {'Content-type': 'application/json'}

    zoneJson = {}
    scheduleJson = {}
    profilesJoson = {}

    option = 0

    while option != 6:
        #os.system('cls' if os.name == 'nt' else 'clear') #use subprocess, not os.system (it's deprecated)
        print("|------------------Welcome to PlanteRGB Lighting System------------------|")

        print("|---------------------------Configuration Menu---------------------------|")

        option = input("Zones (1) | Scheduling (2) | Profiles (3) | Initialize (4)\n| Demo 1 (5) | Demo 2 (6) | Apply Changes(7) | Shutdown (8): ")

        if option == 1:
            print("Selected Zones")
            configure_Zones(url, header, zoneJson)
        elif  option == 2:
            print("Selected Scheduling")
            configure_Schedule(url,header, scheduleJson)
        elif  option == 3:
            print("Profiles Selected")
            configure_profiles(url, header, profilesJoson)
        elif option == 4:
            print("Setting up default Zones, Profiles, and Schedules")
            initialize(url, header)
        elif option == 5:
			print("Crating demo option")
			createDemo1(url, header)
        elif option == 6:
            print("Crating demo option")
            createDemo2(url, header)
        elif  option == 7:
            print("Changes Applied")
            sendToServer(url, header, zoneJson, scheduleJson, profilesJoson)
        elif option == 8:
            shutdown(url, header)

if __name__ == "__main__":
    main() #Startng the main function
