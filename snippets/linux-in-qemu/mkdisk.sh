#!/bin/bash

#
# create disk image
#

if [ $# != 2 ]
then
    printf "Usage:\n"
    printf "\t$0 CONTENTDIR DISKIMG\n\n"
    printf "Example:\n"
    printf "\t$0 diskcontent disk.img\n\n"
    exit 0
fi

contentdir=`readlink -f "$1"`
diskimg=`readlink -f "$2"`
tempdir=`mktemp -d`

fallocate -l 50M "$diskimg"
mkfs.ext4 -F "$diskimg"

sudo mount -t ext4 "$diskimg" "$tempdir"
sudo cp -r "$contentdir/"* "$tempdir"

[ -d "$tempdir"/etc  ] || sudo mkdir "$tempdir"/etc
[ -d "$tempdir"/dev  ] || sudo mkdir "$tempdir"/dev
[ -d "$tempdir"/proc ] || sudo mkdir "$tempdir"/proc
[ -d "$tempdir"/sys  ] || sudo mkdir "$tempdir"/sys
[ -d "$tempdir"/tmp  ] || sudo mkdir "$tempdir"/tmp

sudo chown -R root:root "$tempdir"
sudo umount "$tempdir"
rmdir "$tempdir"
