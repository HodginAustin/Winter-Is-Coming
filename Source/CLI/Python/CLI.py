from datetime import datetime
import requests
import json
import os


def print_request(r):
    print("    status:" + str(r.status_code))
    print("    text:" + r.text)

def initialize(url, header):

    print("Adding Default Profile")
    j = {"name": "Default Profile", "description": "Default Profile"}
    r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=200)

    if(str(r.status_code) == "200"):
        print("Add Successful")


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

    for i  in range(10):
        print("Add LEDS")
        j = {"strip_idx": i, "controller": 1}
        r = requests.post(url + "/leds/add", json=j, headers=header)
        if(str(r.status_code) == "200"):
            print("Add Successful")

    print("Adding  LEDS to Zone1")
    j = range(1,11)
    r = requests.put(url + "/profiles/1/zones/1/leds/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")

    now = datetime.now()
    seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()

    print("Adding White Daily state 100 intensity")
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

    now = datetime.now()
    seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()

    print("Add White Daily State To Zone Starting Now")
    j = [{"time":seconds_since_midnight, "state":1}]
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    if(str(r.status_code) == "200"):
        print("Add Successful")


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

def configure_Zones(url, zoneJson):
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


def createDemo(url, header):

    now = datetime.now()
    seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()

    oneHourLater = now + 3600

    time = now

    #will be state 1
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


    #Will be state 2
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
    #will be state 3
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

    while time != oneHourLater:

        print("Adding Red")

        j = [{"time": time, "state":1}]
        r = requests.post(url + "/daily_states/add", json=j, headers=header)
        print_request(r)

        time = time +10



        print("Adding Green")
        j = [{"time": time, "state":2}]
        r = requests.post(url + "/daily_states/add", json=j, headers=header)
        print_request(r)
        redID = r['id']

        time = time +10


        # print("Adding Blue")
        # j = [{"time": time, "state":3}]
        # r = requests.post(url + "/daily_states/add", json=j, headers=header)
        # print_request(r)
        #
        # time = time +10
        #
        #
        # print("Assign daily state to zone")
        # r = requests.put(url + "/profiles/1/zones/1/day/0/add/1")
        # print_request(r)
        #



def main():
    #URL
    url = "http://localhost:9080"
    # Headers
    header = {'Content-type': 'application/json'}

    zoneJson = {}
    scheduleJson = {}
    profilesJoson = {}

    option = 0

    while option != 6:
        #os.system('cls' if os.name == 'nt' else 'clear')
        print("|------------------Welcome to PlanteRGB Lighting System------------------|")

        print("|---------------------------Configuration Menu---------------------------|")

        option = input("Zones(1) | Scheduling(2) | Profiles(3) | Initialize (4)| Apply Changes(5) | Shutdown(6): ")

        if option == 1:
            print("Selected Zones")
            configure_Zones(url, header,zoneJson)
        elif  option == 2:
            print("Selected Scheduling")
            configure_Schedule(url,header, scheduleJson)
        elif  option == 3:
            print("Profiles Selected")
            configure_profiles(url, header, profilesJoson)
        elif option == 4:
            print("Setting up default Zones, Profiles, and Schedules")
            initialize(url, header)
        elif  option == 5:
            print("Changes Applied")
            sendToServer(url, header, zoneJson, scheduleJson, profilesJoson)
        elif option == 6:
            shutdown(url, header)


if __name__ == "__main__":
    main() #Startng the main function
