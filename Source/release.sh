#!/bin/bash

make clean
make default

tar -cvzf release.tar.gz ./environment/usr/ ./environment/timezone.sh ./out/plantergb