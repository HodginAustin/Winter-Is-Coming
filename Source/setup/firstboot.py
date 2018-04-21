import subprocess
import os
import errno

# NOTE: This script must be executed as superuser in order to do everything it needs to do.
# If you are weary of this (you should be), the commands are listed below, along with comments

# Commands list
commands = [
    "sudo cp /usr/share/zoneinfo/America/Los_Angeles /etc/localtime", # Set timezone
    "sudo raspi-config nonint do_ssh 0", # Enable SSH
    "sudo raspi-config nonint do_serial 0", # Enable Serial
    "sudo raspi-config nonint do_i2c 0", # Enable I2C
    "sudo wget -O /plantergb/release.tar.gz https://github.com/CarvellWakeman/plantergb/releases/download/Latest/release.tar.gz", # Download PlanteRGB
    "sudo tar -xvzf /plantergb/release.tar.gz -C /plantergb", # Unzip files
    #"sudo apt-get update", # Update packages
    #"sudo apt-get -y upgrade", # Upgrade packages
    "sudo apt-get -y install libsqlite3-dev", # Install sqlite package for database
    "sudo cp /plantergb/environment/usr/local/lib/* /usr/local/lib/", # Copy binary files
    "sudo cp -r /plantergb/environment/usr/local/include/* /usr/local/include/", # Copy library files
    "sudo chmod +x /plantergb/out/plantergb", # Allow plantergb process to be executed
    #"sudo apt-get -y install nodejs-legacy", # Install NodeJS for hosting the website
    "sudo wget -O /plantergb/node_install.sh https://github.com/CarvellWakeman/plantergb/releases/download/Latest/node_install.sh", # Install Node
    "sudo chmod +x /plantergb/node_install.sh", # Allow node install script to be executed
    "sudo /plantergb/node_install.sh", # Execute node install script
    "sudo apt-get -y install npm", # Package manager for website
    "sudo npm install --prefix /plantergb/Site", # Install packages for website
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
