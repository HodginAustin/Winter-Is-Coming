#!/bin/bash

make clean
make default

tar --exclude='./Site/node_modules' -cvzf release.tar.gz ./environment/usr ./setup/wifi_reconnect.sh ./setup/node_install.sh ./out/plantergb ./Site/