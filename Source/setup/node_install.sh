#!/bin/bash
# By Steven de Salas
# Modified by Zach Lerew

# Based on script by Richard Stanley @ https://github.com/audstanley/Node-MongoDb-Pi/
# This is for a RaspberryPi Zero but should work across all models.

VERSION=v9.8.0;

# Creates directory for downloads, and downloads node
cd / && mkdir temp && cd temp;
wget https://nodejs.org/dist/$VERSION/node-$VERSION-linux-armv6l.tar.gz;
tar -xzf node-$VERSION-linux-armv6l.tar.gz;
# Remove the tar after extracing it.
sudo rm node-$VERSION-linux-armv6l.tar.gz;
# This next line will copy Node over to the appropriate folder.
sudo cp -R node-$VERSION-linux-armv6l/* /usr/local/;
