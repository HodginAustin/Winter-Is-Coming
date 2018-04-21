import subprocess

# Start plantergb
subprocess.call("sudo /plantergb/out/plantergb &", shell=True)

# Start plantergb website
subprocess.call("sudo npm start --prefix /plantergb/Site &", shell=True)
