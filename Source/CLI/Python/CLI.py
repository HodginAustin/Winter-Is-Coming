from datetime import datetime
import subprocess
import requests
import json
import os
import sys

REQUEST_TIMEOUT =10

NUM_LEDS = 60

system = {}
system['controllers'] = [1, 2] #IO ports
system['profiles'] = {}
system['led_states'] = {}
system['daily_states'] = {}

def get_time():
    now = datetime.now()
    return (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()

def get_dow():
    d = datetime.today().weekday() + 1
    return d if d <= 6 else 0

def print_request(r):
    if(str(r.status_code) == "200"):
        print("Success")
    else:
        print("ERROR:")
        print("    status:" + str(r.status_code))
        print("    text:" + r.text)

def printProfiles(url, header, profilenum):
    r = requests.get(url + "/profiles/%s" %profilenum)
    p = r.json()
    profileCount = 1
    print("-----------------------------------------------------------")
    print("(%s:)" %profilenum)
    print("Name: %s" %p["name"])
    print("Description: %s" %p["description"])
    print("Zones: %s" %p["zones"])
    print("-----------------------------------------------------------")

def getProfiles(url, header):
    r = requests.get(url + "/profiles", "")
    if (str(r.status_code) == "200"):
        return r.json()


def printController(url, header, controlerNum):
    r = requests.get(url + "/controllers/%i" %controlerNum)
    s = r.json()
    if(str(r.status_code) == "200"):
        print("--------------------------")
        print("(%i)" %controlerNum)
        print("Details: %s" %s["details"])
        print("ID: %s" %s["io"] )
        print("--------------------------")
    else:
        print("unable to find controllers")


def initialize(url, header):
    subprocess.call("clear", shell = True);
    print("Adding Default Profile")
    j = {"name": "Default Profile", "description": "Default Profile"}
    r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
    print_request(r)

    print("\nSet current profile")
    j = requests.post(url + "/current_profile/1","", timeout=REQUEST_TIMEOUT)
    print_request(r)

    print("\nAdd Zone to  Default profile 1")
    j = {"name": "Zone 1"}
    r = requests.post(url + "/profiles/1/zones/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
    print_request(r)

    print("\nAdd 1 Controller")
    j = {"io": 1}
    r = requests.post(url +"/controllers/add", json=j, headers=header)
    print_request(r)

    print("\nCreating LEDs")
    for i in range(NUM_LEDS):
        print("Adding LED #%i" % i)
        j = {"strip_idx": i, "controller": 1}
        r = requests.post(url + "/leds/add", json=j, headers=header)
        print_request(r)

    print("\nAdding LEDS to Zone1")
    j = range(1,NUM_LEDS+1)
    r = requests.put(url + "/profiles/1/zones/1/leds/add", json=j, headers=header)
    print_request(r)

    print("\nAdding White LED state 100 intensity")
    j = {
        "r": 255,
        "g": 255,
        "b": 255,
        "intensity": 100,
        "power": True
        }
    r = requests.post(url + "/led_states/add", json=j, headers=header)
    print_request(r)

    print("\nAdd Blank Daily State To Zone Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    print_request(r)

    print("\nAdd Daily State Time Mapping")
    j = [{"time":get_time(), "state":1}]
    r = requests.put(url + "/daily_states/1/led_states/add", json=j, headers=header)
    print_request(r)

    print("\nAssign daily state to zone For Sunday")
    r = requests.put(url + "/profiles/1/zones/1/day/0/add/1")
    print_request(r)

    print("\nCurrent Active state")
    r = requests.get(url + "/profiles/1/zones/1/active_state")
    print_request(r)

    print("\nInitialization Complete")

def shutdown(url, header):
    subprocess.call("clear", shell = True);
    print("Shutting down PlanteRGB Service.....")
    r = requests.get(url + "/shutdown")
    if str(r.status_code) == "200":
        print("Shut down Successful!")

def configure_Zones(url, header):

    option = 0
    option1 = 0
    modifyoption = 0
    subprocess.call("clear", shell = True);
    PL = getProfiles(url, header)
    print("Current Profiles:")
    for p in PL:
        print("-----------------------------------------------------------")
        print("Name: %s" %p["name"])
        print("Description: %s" %p["description"])
        print("Zones: %s" %p["zones"])
        print("-----------------------------------------------------------")

    SelectedProfile = str(raw_input("Which Profile Would you like to edit zones for(1-%s): " %len(PL)))
    while option != 4:
        print("|------------------------Zone Menu------------------------|")
        if len(PL) < 1:
            print("Please run the initialize option on the main menu first")
        else:
            option1 = input("|Add New Zone(1) | Modifiy Zone (2) | Add LEDs To Zone (3)|\n|Go To main Menu (4)|\n|Selection:")

            if option1 == 1:
                return
            elif option1 == 2:
                r = requests.get(url + "/profiles/%i" % option)

                selected_profile = r.json()

                print("-----------------------------------------------------------")
                print("Selected Profile (%i)" % option)
                print("Name: %s" % selected_profile["name"])
                print("Description: %s" % selected_profile["description"])
                print("Zones: %s "% selected_profile["zones"])
                print("-----------------------------------------------------------")
                modifyoption = input("Add Zone To Profile (1) | Add LEDs to Zone (2)|\n|Selection:")

                if option == 1:
                    return
                elif option == 2:
                    return
            elif option1 == 3:
                return

            elif option1 == 4:
                return
    return

def configure_Schedule(url, header):
    subprocess.call("clear", shell = True);

def configure_profiles(url,header):

    subprocess.call("clear", shell = True);
    r = getProfiles(url, header)
    option = 0
    numProfile = 0
    results = []
    temp = 1;

    numProfile = len(r)
    print("num of profiles %i" %numProfile)
    while option != "3":
        temp = 1;
        print("|----------------------------Profile Menu----------------------------|")
        option = str(raw_input("|Add New Profile(1) |Edit Existing Profile (2)|Set Active Profile (3)|\n| Delete Profile(4)|Exit Profile(5)|\n|Selected:"))
        if option == "1":
            print("Adding new profile")
            name = str(raw_input("Please enter the name of new profile: "))
            des = str(raw_input("Please enter a description of the new profile: "))
            j = {"name": name, "description": des}
            r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
            if(str(r.status_code) == "200"):
                print("profile Added!")
        elif option == "2":
            while(temp <= numProfile):
                printProfiles(url, header, temp)
                temp += 1
            option1 = input("Which profile would you like to edit? (1-%i): " % numProfile)
            name = str(raw_input("New Profile Name: "))
            des = str(raw_input("New description:"))
            j = {"name": name, "description": des}
            r = requests.patch(url + "/profiles/%s/edit" %option1, json=j, headers=header, timeout=REQUEST_TIMEOUT)
            if(str(r.status_code) == "200"):
                print("Profile %s updated!" %option1)
        elif option == "3":
            print("-----------------------------------------------------------")
            print("Current Active Profile:")
            r = requests.get(url + "/current_profile")
            if(str(r.status_code) == "200"):
                s = r.json()
                print("Name: %s" %s["name"])
                print("Description: %s" %s["description"])
                print("Zones: %s" %s["zones"])
                print("-----------------------------------------------------------")
            while(temp <= numProfile):
                printProfiles(url, header, temp)
                temp += 1
            option1 = str(raw_input("Acitive Profile:"))
            r = requests.post(url + "/current_profile/%s" %option1)
            if(str(r.status_code) == "200"):
                print("Profile %s set as active!" %option1)
        elif option == "4":
            while temp <= numProfile:
                printProfiles(url, header, temp)
                temp += 1
            option1 = str(raw_input("Which profile would you like to delete?"))
            print("Selected Profile:")
            printProfiles(url, header, option1)
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                   r = requests.delete(url + "/profiles/%s/delete" %option1)
                   if(str(r.status_code) == "200"):
                       print("Profile Deleted!")
        elif option =="5":
            return

def configure_controler(url, header):
    subprocess.call("clear", shell = True)
    PL = getProfiles(url, header)
    option = 0
    optionCount = 1
    r = requests.get(url + "/controllers")
    NumControllers = len(r.json())
    temp = 1

    if len(PL) > 1:
        while option != "4":
            subprocess.call("clear", shell = True)
            optionCount = 1
            temp = 1
            print("|----------------------------Controller Menu----------------------------|")
            option =  str(raw_input("| Add Controller (1) | Edit Controler (2) | Delete Controller (3) |\n| Exit Configure Controler (4): "))

            #add controller
            if option == "1":
                subprocess.call("clear", shell = True)
                while temp <= NumControllers:
                    printController(url, header, temp)
                    temp += 1
                details = str(raw_input("Enter Details of new controllers: "))
                io = str(raw_input("Enter ID of new controler: "))
                j = {"details": details, "io": io}
                r = requests.post(url + "/controllers/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
                print(r)
            #edit controller
            elif option == "2":
                subprocess.call("clear", shell = True)
                while temp <= NumControllers:
                    printController(url, headers, temp)
                    temp += 1
                editOption = str(raw_input("Select which controller you would like to edit(1-%s)" % controllerCount))
                details = str(raw_input("Enter in details: "))
                io = str(raw_input("Enter ID of Controller: "))
                j = {"details":details, "io": io}
                r = requests.patch(url + "/controllers/%i/edit" %editOption, json=j, headers=header, timeout=REQUEST_TIMEOUT)
                if(str(r.status_code) == "200"):
                    print("Controller Successfully Edited!")
            #delete controller
            elif option == "3":
                subprocess.call("clear", shell = True)
                while temp <= NumControllers:
                    printController(url, headers, temp)
                    temp += 1
                deleteoption = str(raw_input("Which controller would you like to delete? (1-%i)" % optionCount))
                printController(url,header, optionCount)
                confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
                if (confirm.lower() == 'y'):
                    r = requests.Delete(url + "/controllers/%i/delete" % optionCount)
    else:
        print("Please run the initialize option to set default profiles")
        return

def configure_LED(url, headerr):
    return

def configure_daily_state(url, header):
    return

def configure_led_state(url, header):
    r = requests.get(url + "/controllers")
    numControl = len(r.json())
    temp = 0
    print("|-------------LED Configuration-------------")
    while temp <= numControl:
        printController(url, header, temp)
        print("Select Controller:")



    return

def configure_demos(url, header):
    subprocess.call("clear", shell = True);
    print("|------------------------Demos----------------------------|")
    option = str(raw_input("Demo 1 (1) | Demo 2 (2) | Demo 3 (3) | Exit Demos (4):"))
    if option == "1":
        print("Creating demo option 1")
        createDemo1(url, header)
    elif option == "2":
        print("Creating demo option 2")
        createDemo2(url, header)
    elif option == "3":
        print("Creating demo option 3")
        # createDemo3(url, header)
    elif option == "4":
        return

def configureHardware(url, header):
    hardwareOption = 0
    while hardwareOption != "3":
        hardwareOption = str(raw_input("Configure Controller (1) | Configure LEDs (2) | Exit Configure Hardware (3)"))
        if hardwareOption == "1":
            configure_controler(url, header)
        elif hardwareOption == "2":
            configure_LED(url, header)

def createDemo1(url, header):
    subprocess.call("clear", shell = True);
    seconds_since_midnight = get_time()
    halfHourLater = seconds_since_midnight + 1800
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
    print_request(r)
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
    print_request(r)

    while time < halfHourLater:
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
        print_request(r)

    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/1/zones/1/day/%i/add/1" % get_dow())
    print_request(r)

def createDemo2(url, header):
    subprocess.call("clear", shell = True);
    seconds_since_midnight = get_time()
    print("Creating Profile 2")
    print("Adding Profile2")
    j = {"name": "Profile2", "description": "Profile2"}
    r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
    print_request(r)
    print("Set current profile")
    j = requests.post(url + "/current_profile/2","", timeout=REQUEST_TIMEOUT)
    if(str(r.status_code) == "200"):
        print("Set current profile successful")
    print("Add Zone 1 to profile2")
    j = {"name": "Zone 1"}
    r = requests.post(url + "/profiles/2/zones/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
    print_request(r)
    print("Adding  LEDS to Zone1")
    j = range(1,NUM_LEDS/2)
    r = requests.put(url + "/profiles/2/zones/2/leds/add", json=j, headers=header)
    print_request(r)

    print("Add Blank Daily State To Zone Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    print_request(r)

    j = []
    print("Adding Red to Zone 1")
    j.append({"time": seconds_since_midnight, "state":2})
    r = requests.put(url + "/daily_states/2/led_states/add", json=j, headers=header)
    print_request(r)

    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/2/zones/2/day/%i/add/2" % get_dow())
    print_request(r)

    j = []

    seconds_since_midnight += 10

def createDemo3(url, header):
    if 'demo3' not in system['profiles']:
        print("Adding Demo3 Profile")
        j = {"name": "Demo 3", "description": "6 zones, split between two controllers"}
        r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        demoProfile = system['profiles']['demo3'] = json.loads(r.text)['id']
        colors = [
            system['led_states']['red'], system['led_states']['green'], system['led_states']['blue'],
            system['led_states']['cyan'], system['led_states']['magenta'], system['led_states']['yellow'],
            system['led_states']['white']
            ]
        leds = [
            range(1,12+1) + [NUM_LEDS + n for n in range(1,12+1)], # First 12 on both strips
            range(12+1,24+1) + [NUM_LEDS + n for n in range(24+1,36+1)], # Second 12 on strip 1, Third 12 on strip 2
            range(24+1,36+1) + [NUM_LEDS + n for n in range(12+1,24+1)], # Third 12 on strip 1, Second 12 on strip 2
            range(36+1,60+1) + [NUM_LEDS + n for n in range(36+1,48+1)], # remaining 24 on strip 1, Fourth 12 on strip 2
            [NUM_LEDS + n for n in range(48+1,60+1)] # remaining 12 on strip 2
            ]
        for z in range(1, 5+1):
            print("Adding Zone to Demo2 Profile, ID:{}".format(demoProfile))
            j = {"name": "Demo 3, Zone {}".format(z+1)}
            r = requests.post(url + "/profiles/{}/zones/add".format(demoProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
            zID = json.loads(r.text)['id']
            print("Adding Daily State {} for Zone {}".format(z+1, z+1))
            j = [{"time": 0, "state": colors[z-1]}]
            r = requests.post(url + "/daily_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
            dsID = system['daily_states']["ds{}".format(z+1)] = json.loads(r.text)['id']
            print("Assigning Daily State {} to Zone {}".format(dsID, zID))
            r = requests.put(url + "/profiles/{}/zones/{}/day/{}/add/{}".format(demoProfile, zID, 0, dsID))
            print_request(r)

            print("Assigning {} LEDS to Zone {}".format(len(leds[z-1]), zID))
            j = leds[z-1]
            r = requests.put(url + "/profiles/{}/zones/{}/leds/add".format(demoProfile, zID), json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
    print("Set current Profile to {}\n".format(system['profiles']['demo3']))
    r = requests.post(url + "/current_profile/{}".format(system['profiles']['demo3']))
    print_request(r)
    print("add zone 2 to profile 2")
    print("Add Zone 2 to  profile2")
    j = {"name": "Zone 2"}
    r = requests.post(url + "/profiles/2/zones/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
    print_request(r)
    print("Adding  LEDS to Zone2")
    j = range(NUM_LEDS/2,NUM_LEDS+1)
    r = requests.put(url + "/profiles/2/zones/3/leds/add", json=j, headers=header)
    print_request(r)
    print("Add Blank Daily State To Zone 2 Starting Now")
    j={}
    r = requests.post(url + "/daily_states/add", json=j, headers=header)
    print_request(r)
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
    print_request(r)
    j = []
    j.append({"time": seconds_since_midnight, "state":5})
    r = requests.put(url + "/daily_states/3/led_states/add", json=j, headers=header)
    print_request(r)
    print("Assign daily state to zone")
    r = requests.put(url + "/profiles/2/zones/3/day/%i/add/3" % get_dow())
    print_request(r)

def configureStates(url, headers):
    return
    stateoption = "0"
    while stateoption != "3":
        stateoption = str(raw_input("Configure Daily State (1) | Configure LED State (2) | Return to main menu (3)"))
        if stateoption == "1":
            configure_daily_state(url, header)
        elif stateoption == "2":
            configure_led_state(url, header)

def nuke_from_orbit(url):
    r = requests.delete(url + "/nuke_from_orbit")
    if r.status_code == 200:
        print("All data erased.\n")

def main():
    #URL
    port = sys.argv[1] if len(sys.argv)>1 else "9080"
    url = "http://localhost:" + port
    # Headers
    header = {'Content-type': 'application/json'}
    option = 0
    while option != "9":
        subprocess.call("clear", shell = True);
        print("|------------------Welcome to PlanteRGB Lighting System------------------|")
        print("|---------------------------Configuration Menu---------------------------|")
        option = str(raw_input("|Zones (1) | Profiles (2 ) | Configure Hardware(3) | Configure States (4)|\n|Initialize (5) | Demos(6) | Erase All(7) | Shutdown (8) | Exit Script(9)|\n|Selection: "))
        if option == "1":
            configure_Zones(url, header)
        elif  option == "2":
            configure_profiles(url, header)
        elif  option == "3":
            configureHardware(url,header)
        elif option == "4":
            configureStates(url, header)
        elif option == "5":
            initialize(url, header)
        elif option == "6":
            configure_demos(url, header)
        elif  option == "7":
            print("This will erase everything in the system. All settings and all data will be gone.")
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                nuke_from_orbit(url)
        elif option == "8":
            shutdown(url, header)
    print("Thank you for using PlanteRGB!")
    sys.exit(0)

if __name__ == "__main__":
    main() #Startng the main function
