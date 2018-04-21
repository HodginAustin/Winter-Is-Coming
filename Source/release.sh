#!/bin/bash

make clean
make default

tar -cvzf release.tar.gz ./environment ./out/plantergb ./Site/ --exclude='node_modules'