#!/bin/bash

make clean
make default

tar --exclude='./Site/node_modules' -cvzf release.tar.gz ./environment/usr ./out/plantergb ./Site/