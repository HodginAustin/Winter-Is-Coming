#!/bin/bash

# List here: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
sudo sh -c "echo $1 > /etc/timezone"
sudo cp /usr/share/zoneinfo/$1 /etc/localtime