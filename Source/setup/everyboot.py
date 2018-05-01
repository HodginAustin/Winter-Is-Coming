import subprocess

# Start plantergb
subprocess.call("cd /plantergb/out/ && ./plantergb &", shell=True)

# Start plantergb website
subprocess.call("npm start --prefix /plantergb/Site &", shell=True)

# Start wifi reconnect monitor
subprocess.call("/plantergb/setup/wifi_reconnect.sh &", shell=True)