import subprocess
import sys
import re


# https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md
def wifi():
    print("\n#--- Select a WIFI network ---#")

    # Print SSIDs
    SSIDS = subprocess.check_output("sudo iwlist wlan0 scan | sed -n '/ESSID/p'", shell=True)
    SSIDS = SSIDS.replace("                    ESSID:", "").replace("\"", "")
    SSIDS = [ssid for ssid in SSIDS.split('\n') if ssid!='']
    
    i = 1
    for SSID in SSIDS:
        print("({}) ".format(i) + SSID)
        i+=1

    # Get input
    usr_select=0
    msg = "\nSelect a network (enter will skip):"
    while usr_select < 1 or usr_select > len(SSIDS):
        usr_select = raw_input(msg)

        if usr_select=="":
            return
        
        try:
            usr_select = int(usr_select)
        except ValueError:
            usr_select = 0
        msg = "Select a value between 1 and {}:".format(len(SSIDS))

    pswd=""
    msg = "Enter the password:"
    while pswd == "":
        pswd = raw_input(msg)
        msg = "Invalid length. Enter the password:"
    
    # Connecting
    print("\nConnecting to network '{}'".format(SSIDS[usr_select-1]))
    wifi_base = 'country=US\n' +\
        'ctrl_interface=DIR=var/run/wpa_supplicant GROUP=netdev\n' +\
        '\n' +\
        'network={\n' +\
        '    ssid=\"%s\"\n' % SSIDS[usr_select-1] +\
        '    psk=\"%s\"\n' % pswd +\
        '}\n'

    # Write to file
    f = open("/boot/wpa_supplicant.conf", "w+")
    f.write(wifi_base)
    f.close()

    # Refresh wifi
    subprocess.call("wpa_cli -i wlan0 reconfigure", shell=True)


def keyboard(layout=""):
    if layout=="":
        print("\n#--- Select a keyboard layout (enter will choose US) ---#")
        
        usr_input=raw_input("Keyboard Layout:")
        layout="us"

        if usr_input != "":
            layout=usr_input
    
    keyboard_base = '# KEYBOARD CONFIGURATION FILE\n' +\
        '# Consult the keyboard(5) manual page.\n' +\
        'XKBMODEL="pc105"\n' +\
        'XKBLAYOUT="%s"\n' % layout +\
        'XKBVARIANT=""\n' +\
        'XKBOPTIONS=""\n' +\
        'BACKSPACE="guess"\n'

    f = open("/etc/default/keyboard", "w+")
    f.write(keyboard_base)
    f.close()


def timezone():
    pattern = re.compile("^[A-Z][^/]+$")

    print("\n#--- Select a timezone (enter will choose America/Los_Angeles) ---#")

    # Continent
    Timezones = subprocess.check_output("find /usr/share/zoneinfo/ -type d", shell=True)
    Timezones = Timezones.replace("/usr/share/zoneinfo/", "")
    Timezones = [tz for tz in Timezones.split('\n') if pattern.match(tz)]

    i = 1
    for Timezone in Timezones:
        print("({}) ".format(i) + Timezone)
        i+=1

    # Get input
    usr_select=0
    msg = "\nSelect a timezone (continent):"
    while usr_select < 1 or usr_select > len(Timezones):
        usr_select = raw_input(msg)

        if usr_select=="":
            return "America", "Los_Angeles"
        
        try:
            usr_select = int(usr_select)
        except ValueError:
            usr_select = 0
        msg = "Select a value between 1 and {}:".format(len(Timezones))

    continent = Timezones[usr_select-1]

    return continent, timezone_region(continent)

def timezone_region(continent):
    pattern = re.compile("^[A-Z].*$")

    # Region
    Timezones = subprocess.check_output("find /usr/share/zoneinfo/{}".format(continent), shell=True)
    print(Timezones)
    Timezones = Timezones.replace("/usr/share/zoneinfo/{}/".format(continent), "")
    Timezones = [tz for tz in Timezones.split('\n') if pattern.match(tz)]

    i = 1
    for Timezone in Timezones:
        print("({}) ".format(i) + Timezone)
        i+=1

    # Get input
    usr_select=0
    msg = "Select a timezone (region):"
    while usr_select < 1 or usr_select > len(Timezones):
        usr_select = raw_input(msg)
        
        try:
            usr_select = int(usr_select)
        except ValueError:
            usr_select = 0
        msg = "Select a value between 1 and {}:".format(len(Timezones))

    return Timezones[usr_select-1]


def user_pass():
    print("\n#--- Enter your new password ---#")
    #pkill -9 -u 'pi'
    subprocess.call("usermod -l plantergb -d /home/plantergb -m pi", shell=True)
    subprocess.call("passwd plantergb", shell=True)


def update():
    print("\n#--- Updating system ---#")
    subprocess.call("apt-get update", shell=True)
    subprocess.call("apt-get upgrade", shell=True)


def colemak():
    print("\n#--- Setting keyboard layout to the best layout, COLEMAK ---#")
    ## Zach only
    subprocess.call("wget https://colemak.com/pub/unix/colemak-1.0.tar.gz", shell=True)
    subprocess.call("gzip -d colemak-1.0.tar.gz", shell=True)
    subprocess.call("tar -xf colemak-1.0.tar", shell=True)
    subprocess.call("cp ./colemak-1.0/xorg/colemak /usr/share/X11/xkb/symbols/colemak", shell=True)
    subprocess.call("rm -rf ./colemak-1.0*", shell=True)
    keyboard("colemak")


def main():
    wifi()
    
    if len(sys.argv) > 1 and sys.argv[1]=="colemak":
        colemak()
    else:
        keyboard()

    continent, region = timezone()
    subprocess.call("cp /usr/share/zoneinfo/{}/{} /etc/localtime".format(continent, region), shell=True)

    user_pass()
    update()

main()

## Optional ##
#touch /boot/ssh # Enables SSH access
