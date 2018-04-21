## Source and Compiler

- This procedure is tested on U-Boot `v2017.01` and maybe applicable for other U-Boot versions.
- Compiler toolchain used is `gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)`.
- The U-Boot source code can be found at [http://www.denx.de/wiki/U-Boot/SourceCode](http://www.denx.de/wiki/U-Boot/SourceCode).
- The compiler toolchain can be found at [https://www.linaro.org/downloads/](https://www.linaro.org/downloads/).

## Enable debug printing

- Add `-DDEBUG` to compiling flag.
- In this U-Boot version, this can be done by adding `-DDEBUG` to line `358` of `Makefile` (change `KBUILD_CPPFLAGS := -D__KERNEL__ -D__UBOOT__` to `KBUILD_CPPFLAGS := -D__KERNEL__ -D__UBOOT__ -DDEBUG`).

## Compiling

```shell
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
make CROSS_COMPILE=arm-linux-gnueabihf-
```

## Flashing

- Replace `<disk>` with your correspond device file (e.g. `sdc`).

```shell
dd if=MLO of=/dev/<disk> bs=512 seek=256 count=256 conv=notrunc
dd if=u-boot.img of=/dev/<disk> bs=512 seek=768 count=1024 conv=notrunc
```

## Copy to FAT16/FAT32 instead of flashing

- BeagleBone Black can also load `MLO` from `FAT16`/`FAT32` primary partition so we can just copy `MLO` and `u-boot.img` to the `FAT16`/`FAT32` primary partition of the micro SD.
- The `FAT16`/`FAT32` partition **must** be a boot partition (active partition).
