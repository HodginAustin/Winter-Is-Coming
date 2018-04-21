import subprocess
import os
import errno

# NOTE: This script must be executed as superuser in order to do everything it needs to do.
# If you are weary of this (you should be), the commands are listed below, along with comments

# Commands list
commands = [
    "cp /usr/share/zoneinfo/America/Los_Angeles /etc/localtime", # Set timezone
    "raspi-config nonint do_ssh 0", # Enable SSH
    "raspi-config nonint do_serial 0", # Enable Serial
    "raspi-config nonint do_i2c 0", # Enable I2C
    "wget -O /plantergb/release.tar.gz https://github.com/CarvellWakeman/plantergb/releases/download/Latest/release.tar.gz", # Download PlanteRGB
    "tar -xvzf /plantergb/release.tar.gz -C /plantergb", # Unzip files
    "apt-get update", # Update packages
    "apt-get -y upgrade", # Upgrade packages
    "apt-get install libsqlite3-dev", # Install sqlite package for database
    "cp /plantergb/environment/usr/local/lib/* /usr/local/lib/", # Copy binary files
    "cp -r /plantergb/environment/usr/local/include/* /usr/local/include/", # Copy library files
    "chmod +x /plantergb/out/plantergb", # Allow plantergb process to be executed
    "apt-get install nodejs-legacy", # Install NodeJS for hosting the website
    "apt-get install npm", # Package manager for website
    "npm install", # Install packages for website
]

# Command output
output = {}

# Logfile
logfile = open("/plantergb_install_log","w+")
logfile.write("") # clear logfile

# Download directory
try:
    os.makedirs("/plantergb")
except OSError as e:
    if e.errno != errno.EEXIST:
        logfile.write("COULD NOT CREATE SETUP DIRECTORY:" + str(e))
        exit()

# Run commands
for cmd in commands:
    try:
        output[cmd] = subprocess.check_output(cmd, shell=True)
        logfile.write(output[cmd])
    except Exception as e:
        logfile.write("Error running command: " + cmd + " " + str(e))
        continue


# Cleanup
logfile.write("SUCCESSFUL INSTALLATION OF PLANTERGB!")
logfile.close()
