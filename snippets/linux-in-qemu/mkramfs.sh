#!/bin/bash

#
# create ramfs file
#

if [ $# != 2 ]
then
    printf "Usage:\n"
    printf "\t$0 CONTENTDIR OUTFILE\n\n"
    printf "Example:\n"
    printf "\t$0 ramfscontent initramfs.cpio\n\n"
    exit 0
fi

contentdir=`readlink -f "$1"`
outfile=`readlink -f "$2"`
curdir=`pwd`

cd "$contentdir"
find . | cpio -o --format=newc > "$outfile"
cd "$curdir"
