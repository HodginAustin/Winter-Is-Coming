import subprocess

# Start plantergb
subprocess.call("cd /plantergb/out/ && ./plantergb &", shell=True)

# Start plantergb website
subprocess.call("npm start --prefix /plantergb/Site &", shell=True)
