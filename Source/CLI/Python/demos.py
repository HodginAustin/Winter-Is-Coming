from datetime import datetime
import requests
import json
import os
import sys

REQUEST_TIMEOUT = 10

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
	if(r.status_code != 200):
		print("ERROR:")
		print("    status:" + str(r.status_code))
		print("    text:" + r.text)


def shutdown(url, header):
	os.system('cls' if os.name == 'nt' else 'clear')
	print("Shutting down PlanteRGB Service.....")
	r = requests.get(url + "/shutdown")
	if str(r.status_code) == "200":
		print("Shut down Successful!")


def nuke_from_orbit(url):
	r = requests.delete(url + "/nuke_from_orbit")
	if r.status_code == 200:
		print("All data erased.\n")


def initialize(url, header):
	NUM_CONTROLLERS = len(system['controllers'])
	
	print("\nAdding {} Controllers".format(NUM_CONTROLLERS))
	for ioPort in system['controllers']:
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

	print("\nAdding White LED state")
	j = {
		"r": 255,
		"g": 255,
		"b": 255,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['white'] = json.loads(r.text)['id']

	print("Adding Red LED state")
	j = {
		"r": 255,
		"g": 0,
		"b": 0,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['red'] = json.loads(r.text)['id']

	print("Adding Green LED state")
	j = {
		"r": 0,
		"g": 255,
		"b": 0,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['green'] = json.loads(r.text)['id']

	print("Adding Blue LED state")
	j = {
		"r": 0,
		"g": 0,
		"b": 255,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['blue'] = json.loads(r.text)['id']

	print("Adding Teal LED state")
	j = {
		"r": 0,
		"g": 128,
		"b": 128,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['teal'] = json.loads(r.text)['id']

	print("Adding Cyan LED state")
	j = {
		"r": 0,
		"g": 255,
		"b": 255,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['cyan'] = json.loads(r.text)['id']

	print("Adding Magenta LED state")
	j = {
		"r": 255,
		"g": 0,
		"b": 255,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['magenta'] = json.loads(r.text)['id']
	
	print("Adding Yellow LED state")
	j = {
		"r": 255,
		"g": 255,
		"b": 0,
		"intensity": 100,
		"power": True
	}
	r = requests.post(url + "/led_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
	print_request(r)
	system['led_states']['yellow'] = json.loads(r.text)['id']


	print("\nInitialization Complete\n")


def createDemo1(url, header):
	if 'demo1' not in system['profiles']:
		print("Adding Demo1 Profile")
		j = {"name": "Demo 1", "description": "One zone, all white"}
		r = requests.post(url + "/profiles/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
		print_request(r)
		demoProfile = system['profiles']['demo1'] = json.loads(r.text)['id']

		print("Adding Zone to Demo1 Profile, ID:{}".format(demoProfile))
		j = {"name": "Demo 1, two controllers, all white"}
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
	if 'demo2' not in system['profiles']:
		# Time
		seconds_since_midnight = get_time()
		time = seconds_since_midnight
		later = seconds_since_midnight + 600
		delay = 5

		print("Adding Demo2 Profile")
		j = {"name": "Demo 2", "description": "Zone 1 = controller 1, zone 2 = controller 2. RGB cycle on 1, CMYK cycle on 2"}
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

		while time < later:
			j = []
			j.append({"time": time, "state": system['led_states']['red']})
			time += delay
			j.append({"time": time, "state": system['led_states']['green']})
			time += delay
			j.append({"time": time, "state": system['led_states']['blue']})
			time += delay
			r = requests.put(url + "/daily_states/{}/led_states/add".format(rgbDailyState), json=j, headers=header)

		print("Adding cmykcycle Daily State ({} min cycle)".format((later-seconds_since_midnight)/60))
		r = requests.post(url + "/daily_states/add", json=j, headers=header, timeout=REQUEST_TIMEOUT)
		print_request(r)
		cmyDailyState = system['daily_states']['cmycycle'] = json.loads(r.text)['id']

		time = seconds_since_midnight
		while time < later:
			j = []
			j.append({"time": time, "state": system['led_states']['cyan']})
			time += delay
			j.append({"time": time, "state": system['led_states']['magenta']})
			time += delay
			j.append({"time": time, "state": system['led_states']['yellow']})
			time += delay
			r = requests.put(url + "/daily_states/{}/led_states/add".format(cmyDailyState), json=j, headers=header)

		print("Assign Daily State {} to Zone {}".format(rgbDailyState, zone1ID))
		r = requests.put(url + "/profiles/{}/zones/{}/day/{}/add/{}".format(demoProfile, zone1ID, get_dow(), rgbDailyState))
		print_request(r)

		print("Assign Daily State {} to Zone {}".format(cmyDailyState, zone1ID))
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


def main():
	#URL
	url = sys.argv[1] if len(sys.argv)>1 else "localhost"
	port = sys.argv[2] if len(sys.argv)>2 else "9080"
	url = "http://" + url + ":" + port

	# Headers
	header = {'Content-type': 'application/json'}

	while True:
		print("|------------------ Welcome to PlanteRGB Lighting System DEMOS ------------------|")
		print("|------------------ Sending data to: {}\n".format(url))

		option = raw_input("Hardware Initialize (1) | Erase All (2) | Shutdown (3)" +
						   "\n| Demo 1 (4) | Demo 2 (5) | Demo 3 (6): ")
		option = int(option or 0)

		if option == 0:
			print("Goodbye!\n")
			exit()
		elif option == 1:
			print("Setting up default Zones, Profiles, and Schedules")
			initialize(url, header)
		elif  option == 2:
			print("This will erase everything in the system. All settings and all data will be gone.")
			confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
			if (confirm.lower() == 'y'):
				nuke_from_orbit(url)
		elif option == 3:
			shutdown(url, header)
			exit()
		elif option == 4:
			createDemo1(url, header)
		elif option == 5:
			createDemo2(url, header)
		elif option == 6:
			createDemo3(url, header)

if __name__ == "__main__":
	main()
