from datetime import datetime
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
	
def demo_system_init():
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
		demo_system_init()


def get_system(url):
	print("Querying system of state")
	# LED states
	r = requests.get(url + "/led_states", timeout=REQUEST_TIMEOUT)
	ledStates = json.loads(r.text)
	for color in colors:
		color['id'] = 0
	for ls in ledStates:
		j = {
			'r': ls['r'],
			'g': ls['g'],
			'b': ls['b'],
			'intensity': ls['intensity'],
			'power': ls['power']
		}

		for color in colors:
			if color['json']['r'] == j['r'] and \
					color['json']['g'] == j['g'] and \
					color['json']['b'] == j['b'] and \
					color['json']['intensity'] == j['intensity'] and \
					color['json']['power'] == j['power']:
				color['id'] = ls['id']
				system['led_states'][color['name']] = ls['id']
	print (system)

	# Profiles
	r = requests.get(url + "/profiles", timeout=REQUEST_TIMEOUT)
	profiles = json.loads(r.text)
	for profile in profiles:
		system['profiles'][profile['name']] = profile['id']

	# Controllers
	r = requests.get(url + "/controllers", timeout=REQUEST_TIMEOUT)
	controllers = json.loads(r.text)
	if (len(controllers) >= len(system['controllers'])):
		system['controllers'] = []
	

def initialize(url, header):
	# Poll for system state
	get_system(url)


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


def createDemo1(url, header):
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


def main():
	#URL
	url = sys.argv[1] if len(sys.argv)>1 else "localhost"
	port = sys.argv[2] if len(sys.argv)>2 else "9080"
	url = "http://" + url + ":" + port

	# Headers
	header = {'Content-type': 'application/json'}

	# Build demo system
	demo_system_init()

	# Load system
	initialize(url, header)
	print("Initialization Complete\n")

	while True:
		print("|------------------ Welcome to PlanteRGB Lighting System DEMOS ------------------|")
		print("|------------------ Sending data to: {}\n".format(url))

		option = raw_input("Reset (1) | Shutdown (2)" +
						   "\n| Demo 1 (3) | Demo 2 (4) | Demo 3 (5): ")
		option = int(option or 0)

		if option == 0:
			print("Goodbye!\n")
			exit()
		elif  option == 1:
			print("This will erase everything in the system. All settings and all data will be gone.")
			confirm = str(raw_input("Do you wish to proceed? [y/N]:"))
			if (confirm.lower() == 'y'):
				nuke_from_orbit(url)
				initialize(url, header)
		elif option == 2:
			shutdown(url, header)
			exit()
		elif option == 3:
			createDemo1(url, header)
		elif option == 4:
			createDemo2(url, header)
		elif option == 5:
			createDemo3(url, header)

if __name__ == "__main__":
	main()
