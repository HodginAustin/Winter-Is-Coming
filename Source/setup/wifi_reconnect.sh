#!/bin/bash

while true ; do
	if ifconfig wlan0 | grep -q "inet " ; then
		sleep 120
	else
		sudo systemctl daemon-reload
        sudo systemctl restart dhcpcd
		sleep 60
	fi
done