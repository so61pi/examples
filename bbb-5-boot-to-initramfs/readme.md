## Preparation

- Assume you already have
  - Linux kernel `uImage`.
  - Device tree blob `am335x-boneblack.dtb`.
  - `initramfs.cpio` or `initramfs.uImage`.

## Load files to RAM

- From micro SD.

```shell
# interface : mmc
# device    : 0
# partition : 2

ext4load mmc 0:2 0x82000000 /uImage
ext4load mmc 0:2 0x88000000 /am335x-boneblack.dtb
ext4load mmc 0:2 0x88080000 /initramfs.cpio # initramfs.uImage if U-Boot image initramfs is used.
```

- From TFTP server.

```shell
setenv autoload no
setenv serverip 192.168.1.10
dhcp
tftp 0x82000000 uImage
tftp 0x88000000 am335x-boneblack.dtb
tftp 0x88080000 initramfs.cpio # initramfs.uImage if U-Boot image initramfs is used.
```

## Boot

- Kernel with internal `initramfs.cpio`.

```shell
setenv bootargs console=ttyO0,115200n8
bootm 0x82000000 - 0x88000000
```

- Kernel with external `initramfs.uImage`.

```shell
setenv bootargs console=ttyO0,115200n8
bootm 0x82000000 0x88080000 0x88000000
```

- Kernel with external `initramfs.cpio`.

```shell
setenv bootargs console=ttyO0,115200n8 initrd=0x88080000,<initramfs.cpio-size>
bootm 0x82000000 - 0x88000000
```
