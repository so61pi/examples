#!/bin/bash

CORES=4
SDIMG="sdcard.img"

CCPREFIX="arm-linux-gnueabihf-"

WORKINGDIR="$HOME/work/tmp/vexpress"
ETCDIR="$WORKINGDIR/etc"

UBOOTSRC="git://git.denx.de/u-boot.git"
UBOOTVER="v2017.05"
UBOOTDIR="$WORKINGDIR/u-boot"

BUSYBOXSRC="git://git.busybox.net/busybox"
BUSYBOXVER="1_26_2"
BUSYBOXDIR="$WORKINGDIR/busybox"

LINUXSRC="git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git"
LINUXVER="v4.11.1"
LINUXDIR="$WORKINGDIR/linux-stable"


OUTPUTDIR="$WORKINGDIR/output"
ROOTFSDIR="$OUTPUTDIR/tmp-rootfs"
SDIMGDIR="$OUTPUTDIR/tmp-sdcard"


panel() {
    echo "********************************************************************************"
    echo $1
    echo "********************************************************************************"
}


subpanel() {
    echo "------------------------------------------------------------"
    echo $1
    echo "------------------------------------------------------------"
}


download() {
    cd "$WORKINGDIR"

    git clone --branch "$UBOOTVER" "$UBOOTSRC"
    git clone --branch "$BUSYBOXVER" "$BUSYBOXSRC"
    git clone --branch "$LINUXVER" "$LINUXSRC"
}


clean() {
    panel "clean"

    rm -rf "$OUTPUTDIR"
    mkdir -p "$OUTPUTDIR"

    cd "$UBOOTDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean

    cd "$BUSYBOXDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean

    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean
}


config() {
    panel "config"

    cd "$UBOOTDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" vexpress_ca9x4_defconfig
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" menuconfig

    cd "$BUSYBOXDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" defconfig
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" menuconfig

    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" distclean
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" vexpress_defconfig
    make ARCH=arm CROSS_COMPILE="$CCPREFIX" menuconfig
}


build() {
    panel "build"

    sudo true

    rm -rf "$OUTPUTDIR"
    mkdir -p "$OUTPUTDIR"

    cd "$UBOOTDIR"
    make -j $CORES ARCH=arm CROSS_COMPILE="$CCPREFIX"
    cp "$UBOOTDIR/u-boot" "$OUTPUTDIR"

    cd "$BUSYBOXDIR"
    make -j $CORES ARCH=arm CROSS_COMPILE="$CCPREFIX" install CONFIG_PREFIX="$ROOTFSDIR"
    mkdir "$ROOTFSDIR/sys"
    mkdir "$ROOTFSDIR/dev"
    mkdir "$ROOTFSDIR/proc"
    mkdir "$ROOTFSDIR/tmp"
    mkdir "$ROOTFSDIR/etc"
    mkdir "$ROOTFSDIR/root"
    mkdir "$ROOTFSDIR/var"
    sudo cp -a "$ETCDIR/." "$ROOTFSDIR/etc"

    cd "$LINUXDIR"
    make -j $CORES ARCH=arm CROSS_COMPILE="$CCPREFIX" all
    cp "$LINUXDIR/arch/arm/boot/dts/vexpress-v2p-ca9.dtb" "$OUTPUTDIR"
    cp "$LINUXDIR/arch/arm/boot/zImage" "$OUTPUTDIR"

    subpanel "Use n to create partitions, use w to save changes and quit"
    cd "$OUTPUTDIR"
    fallocate -l 64M "$SDIMG"
    fdisk "$SDIMG"
    sudo kpartx -a "$SDIMG"
    sleep 3
    ls -alh /dev/mapper

    sudo mkfs.ext3 -F /dev/mapper/loop1p1
    mkdir "$SDIMGDIR"
    sudo mount -t ext3 /dev/mapper/loop1p1 "$SDIMGDIR" -o loop
    sudo cp -a "$ROOTFSDIR/." "$SDIMGDIR"
    tree "$SDIMGDIR"
    sudo umount "$SDIMGDIR"

    sudo kpartx -d "$SDIMG"
}


warning() {
    subpanel "Use CTRL+A X to quit QEMU"
    sleep 3
}


run-uboot() {
    warning

    cd "$OUTPUTDIR"
    qemu-system-arm -M vexpress-a9 -cpu cortex-a9 \
                    -m 512M \
                    -nographic \
                    -kernel "$OUTPUTDIR/u-boot"
}


run-kernel() {
    warning

    cd "$OUTPUTDIR"
    qemu-system-arm -M vexpress-a9 -cpu cortex-a9 \
                    -m 512M \
                    -nographic \
                    -kernel "$OUTPUTDIR/zImage" \
                    -dtb "$OUTPUTDIR/vexpress-v2p-ca9.dtb" \
                    -append "console=ttyAMA0 root=/dev/mmcblk0p1" \
                    -sd "$SDIMG"
}


run() {
    warning

    # mmc dev 0
    # mmc rescan
    # mmcinfo
    # ext4ls mmc 0:1

    # -serial stdio

    cd "$OUTPUTDIR"
    qemu-system-arm -M vexpress-a9 -cpu cortex-a9 \
                    -m 512M \
                    -nographic \
                    -kernel "$OUTPUTDIR/u-boot" \
                    -sd "$SDIMG"
}



while :; do
    case "$1" in
        --download)
            download
            break
            ;;
        --clean)
            clean
            break
            ;;
        --config)
            config
            break
            ;;
        --build)
            build
            break
            ;;
        --run)
            run
            break
            ;;
        --run-uboot)
            run-uboot
            break
            ;;
        --run-kernel)
            run-kernel
            break
            ;;
        -*)
            echo "Unknown flag : $1"
            exit 1
            ;;
        *)
            break
            ;;
    esac
done
