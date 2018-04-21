#!/bin/bash

make clean
make default

tar -cvzf release.tar.gz ./environment/usr/ ./out/plantergb ./Site/