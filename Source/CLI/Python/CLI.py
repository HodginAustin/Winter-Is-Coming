from datetime import datetime
import subprocess
import requests
import json
import os
import sys

REQUEST_TIMEOUT = 10

NUM_LEDS = 60

system = {}
colors = [
	{'name':'white', 'id':0, 'json':{
			"r": 255,
			"g": 255,
			"b": 255,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'red', 'id':0, 'json':{
			"r": 255,
			"g": 0,
			"b": 0,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'green', 'id':0, 'json':{
			"r": 255,
			"g": 0,
			"b": 0,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'blue', 'id':0, 'json':{
			"r": 255,
			"g": 0,
			"b": 0,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'teal', 'id':0, 'json':{
			"r": 0,
			"g": 128,
			"b": 128,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'cyan', 'id':0, 'json':{
			"r": 0,
			"g": 255,
			"b": 255,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'magenta', 'id':0, 'json':{
			"r": 255,
			"g": 0,
			"b": 255,
			"intensity": 100,
			"power": True
		}
	},
	{'name':'yellow', 'id':0, 'json':{
			"r": 255,
			"g": 255,
			"b": 0,
			"intensity": 100,
			"power": True
		}
	}
]

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

def convertTimeString(time_str):
    h, m = time_str.split(':')
    return int(h) * 3600 + int(m) * 60

def print_request(r):
    if(str(r.status_code) == "200"):
        print("Success")
    else:
        print("ERROR:")
        print("    status:" + str(r.status_code))
        print("    text:" + r.text)


def printAllProfiles(url,header):
    r = requests.get(url + "/profiles")
    for p in r.json():
        print("-----------------------------------------------------------")
        print("(%s:)" %p["id"])
        print("Name: %s" %p["name"])
        print("Description: %s" %p["description"])
        print("Zones: %s" %p["zones"])
        print("-----------------------------------------------------------")

def printZones(url,header,profile):
    r = requests.get(url + "/profiles/%s/zones" %profile)
    DOW = ["Sunday" , "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
    i = 0 
    for zone in r.json():
        print("-----------------------")
        print("(%s)" %zone["id"])
        print("Name: %s" %zone["name"])
        i = 0 
        for day in zone["days"]:
            print("{}: {}".format(DOW[i], day))
            i += 1

def printProfiles(url, header, profilenum):
    r = requests.get(url + "/profiles/%s" %profilenum)
    p = r.json()
    print("-----------------------------------------------------------")
    print("(%s:)" %p["id"])
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
    if str(r.status_code) == "200":
        print("--------------------------")
        print("(%i)" %s["id"])
        print("Details: %s" %s["details"])
        print("ID: %s" %s["io"] )
        print("--------------------------")
    else:
        print("unable to find controllers")

def printLEDState(url, headers):
    r = requests.get(url + "/led_states")
    if str(r.status_code) == "200":
        sl = r.json()
        for s in sl:
            print("------------------------")
            print("(%s)" %s["id"])
            print("Red: %s" %s["r"])
            print("Green: %s" %s["g"])
            print("Blue: %s" %s["b"])
            print("intensity: %s" %s["intensity"])
            print("Power: %s" %s["power"])
            print("------------------------")

    else:
        print("Unable to obtain LED states")


def humanize_time(secs):
    mins, secs = divmod(secs, 60)
    hours, mins = divmod(mins, 60)
    return '%02d:%02d:%02d' % (hours, mins, secs)

def printDailyState(url, header):
    r = requests.get(url + "/daily_states")
    sl = r.json()

    for s in sl:
        print("-------------------")
        print("(%s)" %s["id"])
        for l in s["timeStateMap"]:
            print("State ID: %s" %l["state"])
            seconds = int(l["time"])
            time = humanize_time(seconds)
            print("Start Time: %s" %time)
        print("-------------------")

def initialize(url, header):
    NUM_CONTROLLERS = len(system['controllers'])

    for ioPort in system['controllers']:
        print("\nAdding Controller")
        j = {"io": ioPort}
        r = requests.post(url +"/controllers/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        controllerID = json.loads(r.text)['id']

        print("    Creating {} LEDs for controller {}".format(NUM_LEDS, controllerID))
        j = []
        for i in range(NUM_LEDS):
            j.append({"strip_idx": i, "controller": controllerID})
        r = requests.post(url + "/leds/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)

    system['controllers'] = []

    for color in colors:
        if color['id'] == 0:
            print("Adding {} LED state".format(color['name']))
            r = requests.post(url + "/led_states/add", json=color['json'], headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
            j = json.loads(r.text)
            color['id'] = j['id']
            system['led_states'][color['name']] = j['id']

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
    temp = 0
    subprocess.call("clear", shell = True)
    PL = getProfiles(url, header)

    r = requests.get(url + "/daily_states")

    numDailyStates = len(r.json())
    print("Profiles: ")
    printAllProfiles(url,header)
    SelectedProfile = str(raw_input("Which Profile Would you like to edit zones for(1-%s): " %len(PL)))

    while option != "4":
        temp = 0
        numOfDays = 0
        days = []
        subprocess.call("clear", shell = True)
        print("|---------------------------------------- Zone Menu -----------------------------------------|")
        option = str(raw_input("|   Add Zone(1)   |   Assign Daily State(2)   |    Delete (3)   |    Select new profile(3)   |\n| Return to main menu(4) |\n|Selected: "))

        if option == "1":
            subprocess.call("clear", shell = True);
            zoneName = str(raw_input("Name of new Zone:  "))
            des = str(raw_input("Enter in description: "))

            #creating the zone
            j = {"name": zoneName, "description": des}
            r = requests.post(url + "/profiles/{}/zones/add".format(SelectedProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
            zoneID = json.loads(r.text)['id']


            startLED = int(raw_input("Number of first LED in the zone: "))
            endLED = int(raw_input("Number of last LED in the zone: "))

            printDailyState(url, header)
            dailyState = str(raw_input("Select Daily State to apply to zone(1-%s):" %numDailyStates))

             #assigning LED's
            j = range(startLED,endLED)
            r = requests.put(url + "/profiles/{}/zones/{}/leds/add".format(SelectedProfile, zoneID), json=j, headers=header, timeout=REQUEST_TIMEOUT)

        if option == "2":
            subprocess.call("clear", shell = True)
            temp = 0
            days = []
            numOfDays = 0
            subprocess.call("clear", shell = True);
            print("Zones: ")
            printZones(url,header,SelectedProfile)
            selectedZone = str(raw_input("Select Zone: "))
            subprocess.call("clear", shell = True);
            
            print("Daily States: ")
            printDailyState(url,header)

            selectedDailyState = str(raw_input("Select Daily State: "))
            subprocess.call("clear", shell = True);
            numOfDays = int(raw_input("How many days do you want want this zone to apply to? "))
            print("num of days before while: %s" % numOfDays)
            while temp < numOfDays:
                print("Temp: %i" %temp)
                print("num of days in loop: %i" % numOfDays)
                temp1 = str(raw_input("What day as an integer do you want the zone to be active (Sunday(0)-Saturday(6)): "))
                r = requests.put(url + "/profiles/{}/zones/{}/day/{}/add/{}".format(SelectedProfile, selectedZone, temp1, selectedDailyState))
                if(str(r.status_code) == "200"):
                    print("Daily State added!")
                temp += 1

            
        if option == "3":
            subprocess.call("clear", shell = True);
            printZones(url,header,SelectedProfile)
            deleteOption = str(raw_input("Which zone do you want to delete? "))
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                r = requests.delete(url + "/profiles/{}/zones/{}/delete".format(SelectedProfile, deleteOption))
                if(str(r.status_code) == "200"):
                    print("Deleted {} zone from profile {}".format(deleteOption, SelectedProfile))

def configure_profiles(url,header):

    subprocess.call("clear", shell = True)
    r = getProfiles(url, header)
    option = 0
    numProfile = 0
    results = []
    temp = 1;

    numProfile = len(r)
    print("num of profiles %i" %numProfile)
    while option != "5":
        subprocess.call("clear", shell = True)
        temp = 1;
        print("|----------------------------------------Profile Menu----------------------------------------|")
        option = str(raw_input("|  Add(1)  |  Edit(2)  |  Set Active Profile (3)  |  Delete (4)  |   Return to main menu(5)  |\n|Selected:"))
        if option == "1":
            subprocess.call("clear", shell = True)
            name = str(raw_input("Please enter the name of new profile: "))
            des = str(raw_input("Please enter a description of the new profile: "))
            j = {"name": name, "description": des}
            r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
            if(str(r.status_code) == "200"):
                print("profile Added!")
        elif option == "2":
            subprocess.call("clear", shell = True)
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
            subprocess.call("clear", shell = True)
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
            subprocess.call("clear", shell = True)
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
        

def configure_controler(url, header):
    subprocess.call("clear", shell = True)
    PL = getProfiles(url, header)
    option = 0
    optionCount = 1
    r = requests.get(url + "/controllers")
    NumControllers = len(r.json())
    temp = 1
    while option != "5":
        subprocess.call("clear", shell = True)
        optionCount = 1
        temp = 1
        print("|-------------------------------------- Controller Menu -------------------------------------|")
        option =  str(raw_input("|   Add (1)   |  Add LEDs (2)   |  Edit (3)   |   Delete (4)   |   Return to main menu (5)   |\n|Selection: "))
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
        #add LEDs to controller
        if option == "2":
            subprocess.call("clear", shell = True)
            while temp <= NumControllers:
                printController(url, header, temp)
                temp += 1
            SC = str(raw_input("Which controller would you like to add LEDs to?"))
            numled = str(raw_input("How many of LEDS woudld lke to add? "))
            j=[]

            for i in range(int(numled)):
                print("Adding LED #%i" % i)
                j.append({"strip_idx": i, "controller": SC})
            r = requests.post(url + "/leds/add", json=j, headers=header)
            print_request(r)
            #edit controller
        elif option == "3":
            subprocess.call("clear", shell = True)
            while temp <= NumControllers:
                printController(url, header, temp)
                temp += 1
            editOption = str(raw_input("Select which controller you would like to edit(1-%s)" % NumControllers))
            details = str(raw_input("Enter in details: "))
            io = str(raw_input("Enter ID of Controller: "))
            j = {"details":details, "io": io}
            r = requests.patch(url + "/controllers/%s/edit" %editOption, json=j, headers=header, timeout=REQUEST_TIMEOUT)
            if(str(r.status_code) == "200"):
                print("Controller Successfully Edited!")
        #delete controller
        elif option == "4":
            subprocess.call("clear", shell = True)
            while temp <= NumControllers:
                printController(url, header, temp)
                temp += 1
            deleteoption = str(raw_input("Which controller would you like to delete? (1-%i)" % optionCount))
            printController(url,header, optionCount)
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                r = requests.Delete(url + "/controllers/%i/delete" % optionCount)

def configure_daily_state(url, header):
    option = "0"
    suboption = "0"
    loopCount = 0

    r = requests.get(url + "/led_states")
    numLEDStates = len(r.json())
    r = requests.get(url + "/daily_states")
    numDailyStates = len(r.json())

    while option != "4":
        subprocess.call("clear", shell = True)
        loopCount = 0
        print("|--------------------------------- Daily State Configuration --------------------------------|")
        option = str(raw_input("|     Add (1)     |     Edit (2)     |     Delete (3)      |      Return to Main Menu(4)     |\n| Selected: "))

        if option == "1":
            subprocess.call("clear", shell = True)
            j = []
            suboption = str(raw_input("How many dailystates would you like to add? "))

            while loopCount < int(suboption):
                printDailyState(url, header)
                time = str(raw_input("Enter in time to start (HH:MM) (0-24):(0-59): "))
                seconds_From_Midnight = convertTimeString(time)
                printLEDState(url,header)
                state = str(raw_input("LED State(0-%s):" %numLEDStates ))
                j.append({"time": seconds_From_Midnight, "state": int(state)})
                loopCount += 1

            print("J: %s" %j)
            r = requests.post(url + "/daily_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)

        if option == "2":
            subprocess.call("clear", shell = True)
            while suboption != "3":
                loopCount = 0
                printDailyState(url, header)
                suboption = str(raw_input("|     Add State/Time to Existing Daily State (1)     |     Edit Existing Daily State (2)     |\n| Return to Daily State Menu (3) |\n|Selection: "))

                if suboption == "1":
                    subprocess.call("clear", shell = True)                    
                    D_State = str(raw_input("Which daily state would you like to add to? (1-%s)" %numDailyStates))

                    j = []
                    numDailyAdd = str(raw_input("How many dailystates would you like to add? "))

                    printDailyState(url, header)
                    while loopCount < int(numDailyAdd):

                        time = str(raw_input("Enter in time to start (HH:MM) (0-24):(0-59): "))
                        seconds_From_Midnight = convertTimeString(time)
                        printLEDState(url,header)
                        state = str(raw_input("LED State(0-%s):" %numLEDStates ))
                        j.append({"time": seconds_From_Midnight, "state": int(state)})
                        loopCount += 1

                    r = requests.put(url + "/daily_states/%s/led_states/add" %D_State, json=j, headers=header, timeout=REQUEST_TIMEOUT)
                    print_request(r)


                if suboption == "2":
                    subprocess.call("clear", shell = True)
                    printDailyState(url, header)
                    D_State = str(raw_input("Which daily state would you like to edit? (1-%s)" %numDailyStates))

                    j = []
                    numDailyAdd = str(raw_input("How many daily states would you like to add? "))

                    while loopCount < int(numDailyAdd):
                        printDailyState(url, header)
                        time = str(raw_input("Enter in time to start (HH:MM) (0-24):(0-59): "))
                        seconds_From_Midnight = convertTimeString(time)
                        printLEDState(url,header)
                        state = str(raw_input("LED State(0-%s):" %numLEDStates ))
                        j.append({"time": seconds_From_Midnight, "state": int(state)})
                        loopCount += 1

                    r = requests.patch(url + "/daily_states/%s/edit" %D_State, json=j, headers=header, timeout=REQUEST_TIMEOUT)
                    print_request(r)
        if option == "3":
            subprocess.call("clear", shell = True)
            printDailyState(url, header)
            option1 = str(raw_input("Which Daily State Would you like to delete? (1-%s)" %numDailyStates))
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                   r = requests.delete(url + "/daily_states/%s/delete" %option1)
                   if(str(r.status_code) == "200"):
                       print("Daily State Deleted!")

def configure_led_state(url, header):
    temp = 1
    option = "0"
    while option != "4":
        print("|--------------------------------- LED State Configuration ----------------------------------|")
        option = str(raw_input("|     Add (1)     |     Edit(2)      |      Delete (3)     |     Return to Main Menu (4)     |\n|Selection:"))
        if option == "1":
            red = str(raw_input("Enter Red Value (0-255): "))
            green = str(raw_input("Enter Green Value ( 0-255): "))
            blue = str(raw_input("Enter Blue Value (0-255): "))
            intensity = str(raw_input("Enter in Brightness (0-100): "))
            power = str(raw_input("Enter Power(true/false): "))
            j = {
                "r": int(red),
                "g": int(green),
                "b": int(blue),
                "intensity": int(intensity),
                "power": bool(power)
                }
            r = requests.post(url + "/led_states/add", json=j, headers=header)
            print_request(r)

        if option == "2":
            printLEDState(url,header)
            editOption = str(raw_input("Which LED State Would you like to edit? "))

            red = str(raw_input("Enter New Red Value (0-255): "))
            green = str(raw_input("Enter New Green Value ( 0-255): "))
            blue = str(raw_input("Enter New Blue Value (0-255): "))
            intensity = str(raw_input("Enter New in Brightness (0-100):"))
            power = str(raw_input("Enter New Power(True/False:" ))
            j = {
                "r": int(red),
                "g": int(green),
                "b": int(blue),
                "intensity": int(intensity),
                "power": bool(power)
            }
            r = requests.patch(url + "/led_states/%s/edit" %editOption, json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
        if option == "3":
            printLEDState(url, header)
            deleteOption = str(raw_input("Which LED state would you like to delete? "))
            confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
            if (confirm.lower() == 'y'):
                r = requests.delete(url + "/led_states/%s/delete" %deleteOption)
                if str(r.status_code) == "200":
                    print("Deleted LED State %i" %deleteOption)

def configure_demos(url, header):
    subprocess.call("clear", shell = True);
    while option != 4:
        print("|---------------------------------------- Demos --------------------------------------------|")
        option = str(raw_input("|     Demo 1 (1)     |      Demo 2 (2)     |       Demo 3 (3)     |       Exit Demos (4)    |\n|Selection:"))
        if option == "1": 
            print("Creating demo option 1")
            createDemo1(url, header)
        elif option == "2":
            print("Creating demo option 2")
            createDemo2(url, header)
        elif option == "3":
            print("Creating demo option 3")
            createDemo3(url, header)

def createDemo1(url, header):
    subprocess.call("clear", shell = True);
    if 'demo1' not in system['profiles']:
        print("Adding Demo1 Profile")
        j = {"name": "demo1", "description": "One zone, all white"}
        r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        demoProfile = system['profiles']['demo1'] = json.loads(r.text)['id']

        print("Adding Zone to Demo1 Profile, ID:{}".format(demoProfile))
        j = {"name": "demo1-zone", "description": "Demo 1, two controllers, all white"}
        r = requests.post(url + "/profiles/{}/zones/add".format(demoProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        zone1ID = json.loads(r.text)['id']

        print("Adding fullwhite Daily State")
        j = [{"time": get_time(), "state": system['led_states']['white']}]
        r = requests.post(url + "/daily_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        system['daily_states']['fullwhite'] = json.loads(r.text)['id']

        print("Assigning Daily State {} to Zone {} on Sunday".format(system['daily_states']['fullwhite'], zone1ID))
        r = requests.put(url + "/profiles/{}/zones/{}/day/0/add/{}".format(demoProfile, zone1ID, system['daily_states']['fullwhite']))
        print_request(r)

        print("Assigning {} LEDS to Zone {}".format(NUM_LEDS*2, zone1ID))
        j = range(1,(NUM_LEDS*2)+1)
        r = requests.put(url + "/profiles/{}/zones/{}/leds/add".format(demoProfile, zone1ID), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)

    print("Set current Profile to {}\n".format(system['profiles']['demo1']))
    r = requests.post(url + "/current_profile/{}".format(system['profiles']['demo1']),"", timeout=200)
    print_request(r)

def createDemo2(url, header):
    subprocess.call("clear", shell = True);
    if 'demo2' not in system['profiles']:
        how_long = raw_input("How long should it run? (minutes): ")
        how_long = int(how_long or 10) # 10 mins default

        # Time
        seconds_since_midnight = get_time()
        time = seconds_since_midnight
        later = seconds_since_midnight + (how_long*60)
        delay = 5

        print("Adding Demo2 Profile")
        j = {"name": "demo2", "description": "Zone 1 = controller 1, zone 2 = controller 2. RGB cycle on 1, CMYK cycle on 2"}
        r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        demoProfile = system['profiles']['demo2'] = json.loads(r.text)['id']

        print("Adding Zone to Demo2 Profile, ID:{}".format(demoProfile))
        j = {"name": "Demo 2, Zone 1, RGB cycle"}
        r = requests.post(url + "/profiles/{}/zones/add".format(demoProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        zone1ID = json.loads(r.text)['id']

        print("Adding Zone to Demo2 Profile, ID:{}".format(demoProfile))
        j = {"name": "Demo 2, Zone 2, CMY cycle"}
        r = requests.post(url + "/profiles/{}/zones/add".format(demoProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        zone2ID = json.loads(r.text)['id']

        print("Adding rgbcycle Daily State ({} min cycle)".format((later-seconds_since_midnight)/60))
        r = requests.post(url + "/daily_states/add", json={}, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        rgbDailyState = system['daily_states']['rgbcycle'] = json.loads(r.text)['id']

        c = 0
        j = []
        while time < later:
            j.append({"time": time, "state": system['led_states']['red']})
            time += delay
            j.append({"time": time, "state": system['led_states']['green']})
            time += delay
            j.append({"time": time, "state": system['led_states']['blue']})
            time += delay
            c+=3
            if c > 50:
                c = 0
                r = requests.put(url + "/daily_states/{}/led_states/add".format(rgbDailyState), json=j, headers=header)
                j = []

        print("Adding cmykcycle Daily State ({} min cycle)".format((later-seconds_since_midnight)/60))
        r = requests.post(url + "/daily_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        cmyDailyState = system['daily_states']['cmycycle'] = json.loads(r.text)['id']

        time = seconds_since_midnight
        c = 0
        j = []
        while time < later:
            j = []
            j.append({"time": time, "state": system['led_states']['cyan']})
            time += delay
            j.append({"time": time, "state": system['led_states']['magenta']})
            time += delay
            j.append({"time": time, "state": system['led_states']['yellow']})
            time += delay
            c+=3
            if c > 50:
                c = 0
                r = requests.put(url + "/daily_states/{}/led_states/add".format(cmyDailyState), json=j, headers=header)
                j = []

        print("Assign Daily State {} to Zone {}".format(rgbDailyState, zone1ID))
        r = requests.put(url + "/profiles/{}/zones/{}/day/{}/add/{}".format(demoProfile, zone1ID, get_dow(), rgbDailyState))
        print_request(r)

        print("Assign Daily State {} to Zone {}".format(cmyDailyState, zone2ID))
        r = requests.put(url + "/profiles/{}/zones/{}/day/{}/add/{}".format(demoProfile, zone2ID, get_dow(), cmyDailyState))
        print_request(r)

        print("Assigning {} LEDS to Zone {}".format(NUM_LEDS, zone1ID))
        j = range(1,NUM_LEDS+1)
        r = requests.put(url + "/profiles/{}/zones/{}/leds/add".format(demoProfile, zone1ID), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)

        print("Assigning {} LEDS to Zone {}".format(NUM_LEDS, zone2ID))
        j = range(NUM_LEDS+1,(NUM_LEDS*2)+1)
        r = requests.put(url + "/profiles/{}/zones/{}/leds/add".format(demoProfile, zone2ID), json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)

    print("Set current Profile to {}\n".format(system['profiles']['demo2']))
    r = requests.post(url + "/current_profile/{}".format(system['profiles']['demo2']))
    print_request(r)

def createDemo3(url, header):
    if 'demo3' not in system['profiles']:
        print("Adding Demo3 Profile")
        j = {"name": "demo3", "description": "6 zones, split between two controllers"}
        r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
        print_request(r)
        demoProfile = system['profiles']['demo3'] = json.loads(r.text)['id']

        leds = [
            range(1,12+1) + [NUM_LEDS + n for n in range(1,12+1)], # First 12 on both strips
            range(12+1,24+1) + [NUM_LEDS + n for n in range(24+1,36+1)], # Second 12 on strip 1, Third 12 on strip 2
            range(24+1,36+1) + [NUM_LEDS + n for n in range(12+1,24+1)], # Third 12 on strip 1, Second 12 on strip 2
            range(36+1,60+1) + [NUM_LEDS + n for n in range(36+1,48+1)], # remaining 24 on strip 1, Fourth 12 on strip 2
            [NUM_LEDS + n for n in range(48+1,60+1)] # remaining 12 on strip 2
        ]
        color_order = [
            system['led_states']['red'], system['led_states']['green'], system['led_states']['blue'],
            system['led_states']['cyan'], system['led_states']['magenta'], system['led_states']['yellow'],
            system['led_states']['white']
        ]
        for z in range(1, 5+1):
            print("Adding Zone to Demo2 Profile, ID:{}".format(demoProfile))
            j = {"name": "Demo 3, Zone {}".format(z+1)}
            r = requests.post(url + "/profiles/{}/zones/add".format(demoProfile), json=j, headers=header, timeout=REQUEST_TIMEOUT)
            print_request(r)
            zID = json.loads(r.text)['id']

            print("Adding Daily State {} for Zone {}".format(z+1, z+1))
            j = [{"time": 0, "state": color_order[z-1]}]
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

def configureStates(url, header):
    stateoption = 0
    while stateoption != "3":
        print("|--------------------------------- State Configuration Menu ---------------------------------|")
        stateoption = str(raw_input("|   Configure Daily State (1)   |   Configure LED State (2)   |    Return to main menu (3)   |\n|Selection:"))
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
        print("|--------------------------- Welcome to PlanteRGB Lighting System ---------------------------|")
        print("|------------------------------------ Configuration Menu ------------------------------------|")
        option = str(raw_input("|     Profiles (1)     | Zones (2) | Configure controller (3) |     Configure States (4)     |\n| Initialize Demos (5) | Demos(6)  |       Erase All(7)       | Shutdown (8) | Exit Script(9)|\n|Selection: "))

        if  option == "1":
            configure_profiles(url, header)
        elif option == "2":
            configure_Zones(url, header)
        elif  option == "3":
             configure_controler(url, header)
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
