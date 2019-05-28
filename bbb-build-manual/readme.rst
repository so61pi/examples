.. contents:: Table of Contents
.. section-numbering::

Compile And Flash U-Boot
========================

Source and Compiler
-------------------

- This procedure is tested on U-Boot ``v2017.01`` and maybe applicable for other U-Boot versions.
- Compiler toolchain used is ``gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)``.
- The U-Boot source code can be found at http://www.denx.de/wiki/U-Boot/SourceCode.
- The compiler toolchain can be found at https://www.linaro.org/downloads/.

Enable Debug Printing
---------------------

- Add ``-DDEBUG`` to compiling flag.
- In this U-Boot version, this can be done by adding ``-DDEBUG`` to line ``358`` of ``Makefile`` \.

  * Change ``KBUILD_CPPFLAGS := -D__KERNEL__ -D__UBOOT__`` to ``KBUILD_CPPFLAGS := -D__KERNEL__ -D__UBOOT__ -DDEBUG``.

Compile
-------

.. code-block:: shell

    make CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
    make CROSS_COMPILE=arm-linux-gnueabihf-

Flash
-----

- Replace ``<disk>`` with your correspond device file (e.g. ``sdc``).

.. code-block:: shell

    dd if=MLO of=/dev/<disk> bs=512 seek=256 count=256 conv=notrunc
    dd if=u-boot.img of=/dev/<disk> bs=512 seek=768 count=1024 conv=notrunc

Copy To FAT16/FAT32 Instead Of Flashing
---------------------------------------

- BeagleBone Black can also load ``MLO`` from ``FAT16/FAT32`` primary partition so we can just copy ``MLO`` and ``u-boot.img`` to the ``FAT16/FAT32`` primary partition of the micro SD.
- The ``FAT16/FAT32`` partition **must** be a boot partition (active partition).

Compile BusyBox
===============

Source
------

- This procedure is tested on busybox ``1.26.2`` and maybe applicable for other busybox versions.
- The busybox source code can be found at https://busybox.net/downloads/.

Compile
-------

- We compile busybox with static library option.

.. code-block:: shell

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- defconfig

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
        # Busybox Settings -> Build Options -> Build BusyBox as a static library (no shared libs)

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX=/tmp/busybox

Prepare ``initramfs``
=====================

Preparation
-----------

- Assume ``/tmp/initramfs/`` already has busybox.

Build ``rootfs``
----------------

.. code-block:: shell

    cd /tmp/initramfs
    mkdir dev
    chmod 755 dev
    mknod -m 600 dev/console c 5 1

    mkdir root
    chmod 700 root
    cd -

Create ``/init``
----------------

- Below is the content of ``/tmp/initramfs/init`` file.

.. code-block:: shell

    #!/bin/sh

    mount -t devtmpfs none /dev

    [ -d /sys ]  || mkdir /sys
    [ -d /proc ] || mkdir /proc
    [ -d /tmp ]  || mkdir /tmp
    mount -t sysfs sysfs /sys
    mount -t proc proc /proc
    mount -t tmpfs tmpfs /tmp

    /bin/sh

- Make it executable.

.. code-block:: shell

    chmod +x /tmp/initramfs/init

Create ``initramfs.cpio``
-------------------------

.. code-block:: shell

    cd /tmp/initramfs
    find . | cpio -o --format=newc > initramfs.cpio

Create ``initramfs.uImage``
---------------------------

- If we want to boot with initramfs in uImage, we can create ``initramfs.uImage`` with the following command.

.. code-block:: shell

    mkimage -A arm -O linux -T ramdisk -C none -d initramfs.cpio initramfs.uImage

Compile Linux Kernel
====================

Source
------

- This procedure is tested on linux ``4.4`` and maybe applicable for other linux versions.
- The linux source code can be found at https://github.com/beagleboard/linux.

Compile Without ``initramfs.cpio``
----------------------------------

.. code-block:: shell

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000

Compile With ``initramfs.cpio``
-------------------------------

.. code-block:: shell

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
      # General setup -> Initial RAM filesystem and RAM disk (initramfs/initrd) support -> Initramfs source file(s)
      # This changes the CONFIG_INITRAMFS_SOURCE flag.

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000

Output
------

- The linux kernel in U-Boot image format is at ``arch/arm/boot/uImage``.
- The output dtb is at ``arch/arm/boot/dts/am335x-boneblack.dtb``.

Compile And Install Modules
---------------------------

.. code-block:: shell

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=/tmp/rootfs

Boot To ``initramfs``
=====================

Preparation
-----------

- Assume you already have

  - Linux kernel ``uImage``.
  - Device tree blob ``am335x-boneblack.dtb``.
  - ``initramfs.cpio`` or ``initramfs.uImage``.

Load Files To RAM
-----------------

- From micro SD.

.. code-block:: shell

    # interface : mmc
    # device    : 0
    # partition : 2

    ext4load mmc 0:2 0x82000000 /uImage
    ext4load mmc 0:2 0x88000000 /am335x-boneblack.dtb
    ext4load mmc 0:2 0x88080000 /initramfs.cpio # initramfs.uImage if U-Boot image initramfs is used.

- From TFTP server.

.. code-block:: shell

    setenv autoload no
    setenv serverip 192.168.1.10
    dhcp
    tftp 0x82000000 uImage
    tftp 0x88000000 am335x-boneblack.dtb
    tftp 0x88080000 initramfs.cpio # initramfs.uImage if U-Boot image initramfs is used.

Boot
----

- Kernel with internal ``initramfs.cpio``.

.. code-block:: shell

    setenv bootargs console=ttyO0,115200n8
    bootm 0x82000000 - 0x88000000

- Kernel with external ``initramfs.uImage``.

.. code-block:: shell

    setenv bootargs console=ttyO0,115200n8
    bootm 0x82000000 0x88080000 0x88000000

- Kernel with external ``initramfs.cpio``.

.. code-block:: shell

    setenv bootargs console=ttyO0,115200n8 initrd=0x88080000,<initramfs.cpio-size>
    bootm 0x82000000 - 0x88000000
