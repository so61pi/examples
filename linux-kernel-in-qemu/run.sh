#!/bin/bash

#
# run qemu
#

if [ $# != 2 ] && [ $# != 3 ]
then
    printf "Ussage:\n"
    printf "\t$0 VMLINUZ INITRAMFS [HDA]\n\n"
    printf "Example:\n"
    printf "\t$0 bzImage initramfs.cpio\n"
    printf "\t$0 bzImage initramfs.cpio disk.img\n\n"
    exit 0
fi

vmlinuz=`readlink -f "$1"`
initramfs=`readlink -f "$2"`
diskimg=`readlink -f "$3"`

# check arguments
if [ ! -f "$vmlinuz" ]
then
    printf "$vmlinuz is not a file.\n"
    exit -1
fi

if [ ! -f "$initramfs" ]
then
    printf "$initramfs is not a file.\n"
    exit -1
fi

if [ ! -z "$diskimg" ] && [ ! -f "$diskimg" ]
then
    printf "$diskimg is not a file.\n"
    exit -1
fi

# run
printf "*****                                                      *****\n"
printf "*                                                              *\n"
printf "* When using qemu with -nographic, press Ctrl+A H to get help. *\n"
printf "*                                                              *\n"
printf "*****                                                      *****\n"

sleep 2

if [ -z "$diskimg" ]
then
    qemu-system-x86_64 -kernel "$vmlinuz" -initrd "$initramfs" -nographic -append "console=ttyS0"
else
    qemu-system-x86_64 -kernel "$vmlinuz" -initrd "$initramfs" -hda "$diskimg" -nographic -append "console=ttyS0"
fi
