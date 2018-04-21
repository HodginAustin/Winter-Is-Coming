import subprocess

# Start plantergb
subprocess.call("/plantergb/out/plantergb &", shell=True)

# Start plantergb website
subprocess.call("npm start --prefix /plantergb/Site &", shell=True)
