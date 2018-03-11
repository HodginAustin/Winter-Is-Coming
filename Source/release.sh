#!/bin/bash

make clean
make default

tar -cvzf release.tar.gz ./envirionment/usr/ ./environment/timezone.sh ./out/plantergb