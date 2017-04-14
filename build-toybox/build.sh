#!/bin/bash

# Software
#   toybox 0.7.3
#   http://landley.net/toybox/downloads/toybox-0.7.3.tar.gz

# Use make help for build instructions.
make distclean
make defconfig
CROSS_COMPILE=arm-none-linux-gnueabihf- CC=gcc make
make PREFIX="$HOME/work/tmp/toybox" install
