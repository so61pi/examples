#!/bin/bash

WORKINGDIR="$HOME/work/tmp/bbb"

UBOOTDIR="$WORKINGDIR/u-boot-2017.01"
BUSYBOXDIR="$WORKINGDIR/busybox-1.26.2"
LINUXDIR="$WORKINGDIR/linux"
RUNTIMEDIR="$WORKINGDIR/runtime"
INITFILE="$WORKINGDIR/init"
LINUXCONFIGFILE="$WORKINGDIR/config"

OUTPUTDIR="$WORKINGDIR/output"
ROOTFSDIR="$WORKINGDIR/tmp-rootfs"
INITRAMFSDIR="$WORKINGDIR/tmp-initramfs"


uboot-config() {
    cd "$UBOOTDIR"
    make CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
    make CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
}


uboot-build() {
    cd "$UBOOTDIR"
    make CROSS_COMPILE=arm-linux-gnueabihf-
}


uboot-install() {
    cd "$UBOOTDIR"
    cp MLO "$OUTPUTDIR"
    cp u-boot.img "$OUTPUTDIR"
}


busybox-config() {
    cd "$BUSYBOXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
}


busybox-build() {
    cd "$BUSYBOXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
}


busybox-install() {
    cd "$BUSYBOXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX="$INITRAMFSDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX="$ROOTFSDIR"
}


initramfs-create() {
    cd "$INITRAMFSDIR"
    mkdir dev
    chmod 755 dev
    sudo mknod -m 600 dev/console c 5 1
    mkdir -p root
    chmod 700 root
}


initramfs-install() {
    cd "$INITRAMFSDIR"
    find . | cpio -o --format=newc | gzip > initramfs_data.cpio.gz
    mkdir -p "$LINUXDIR/usr"
    mv initramfs_data.cpio.gz "$LINUXDIR/usr"
}


linux-config() {
    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
}


linux-build() {
    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000
}


linux-build-modules() {
    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
}


linux-install() {
    cd "$LINUXDIR"
    cp arch/arm/boot/uImage "$OUTPUTDIR"
    cp arch/arm/boot/dts/am335x-boneblack.dtb "$OUTPUTDIR"
}

linux-install-modules() {
    cd "$LINUXDIR"
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH="$ROOTFSDIR"
}


rootfs-create-squashfs() {
    mkdir -p "$ROOTFSDIR/dev"
    mkdir -p "$ROOTFSDIR/etc"
    mkdir -p "$ROOTFSDIR/proc"
    mkdir -p "$ROOTFSDIR/root"
    mkdir -p "$ROOTFSDIR/sys"
    mkdir -p "$ROOTFSDIR/tmp"
    mkdir -p "$ROOTFSDIR/var"
    mksquashfs "$ROOTFSDIR" "$OUTPUTDIR/rootfs.sqsh"
}


tftp-install() {
    if [ -d "/tftpboot" ]; then
        cp "$OUTPUTDIR/uImage" /tftpboot
        cp "$OUTPUTDIR/am335x-boneblack.dtb" /tftpboot
    fi
}


bbb-config() {
    rm -rf "$OUTPUTDIR"
    rm -rf "$INITRAMFSDIR"
    rm -rf "$ROOTFSDIR"

    uboot-config
    busybox-config
    linux-config
}


bbb-build() {
    mkdir -p "$OUTPUTDIR"
    mkdir -p "$INITRAMFSDIR"
    mkdir -p "$ROOTFSDIR"

    cp "$INITFILE" "$INITRAMFSDIR"
    cp -R "$RUNTIMEDIR" "$ROOTFSDIR"

    uboot-build
    busybox-build
    busybox-install
    initramfs-create
    initramfs-install
    linux-build
    linux-install
    linux-build-modules
    linux-install-modules
    rootfs-create-squashfs
    tftp-install
}


while :; do
    case "$1" in
        --config)
            bbb-config
            break
            ;;
        --build)
            bbb-build
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
