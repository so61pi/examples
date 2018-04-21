## Source

- This procedure is tested on linux `4.4` and maybe applicable for other linux versions.
- The linux source code can be found at [https://github.com/beagleboard/linux](https://github.com/beagleboard/linux).

## Compiling without `initramfs.cpio`

```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000
```

## Compiling with `initramfs.cpio`

```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
	# General setup -> Initial RAM filesystem and RAM disk (initramfs/initrd) support -> Initramfs source file(s)
	# This changes the CONFIG_INITRAMFS_SOURCE flag.

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000
```

## Output

- The linux kernel in U-Boot image format is at `arch/arm/boot/uImage`.
- The output dtb is at `arch/arm/boot/dts/am335x-boneblack.dtb`.

## Compile and install modules

```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=/tmp/rootfs
```
