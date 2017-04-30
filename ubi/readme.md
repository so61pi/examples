**Reference**

- http://www.linux-mtd.infradead.org/doc/ubi.html
- http://www.linux-mtd.infradead.org/faq/general.html
- http://free-electrons.com/blog/managing-flash-storage-with-linux/
- drivers/mtd/nand/
- drivers/mtd/ubi/


**Overview**

"MTD subsystem (stands for Memory Technology Devices) provides an abstraction layer for raw flash devices. It makes it possible to use the same API when working with different flash types and technologies, e.g. NAND, OneNAND, NOR, AG-AND, ECC'd NOR, etc."

`/dev/mtdX` presents an MTD partition (it's also called an MTD device).

There is nothing like `/dev/mtd` for MTD device (like the relationship between `/dev/sda` and `/dev/sda0`). If the MTD device has 3 partitions, they are `/dev/mtd0`, `/dev/mtd1` and `/dev/mtd2`, and that's it.

There is also no MTD partition table in the MTD device, it's defined in the device tree or via kernel parameter `mtdparts`.

The UBI is "a volume management system for raw flash devices which manages multiple logical volumes on a single physical flash device and spreads the I/O load (i.e, wear-leveling) across whole flash chip". It works on top of the MTD sub-system.

`/dev/ubi0` is called a UBI device.
UBI supports read-only block devices (http://www.linux-mtd.infradead.org/faq/ubi.html#L_squashfs_over_ubi).
UBI doesn't suppoer read-write block devices (http://www.linux-mtd.infradead.org/faq/ubi.html#L_ext2_over_ubi).


**nandsim**

```shell
# Create MTD device with
#   size : 128MB,
#   partition 1 : 32MB
#   partition 2 : 32MB
#   partition 3 : the rest
# `first_id_byte`   : manufacture id
# `second_id_byte`  : device id
# `third_id_byte` and `fourth_id_byte` usage can be found at `nand_decode_ext_id` function.
#
# The supported flash chips are listed at `drivers/mtd/nand/nand_ids.c`.
# Parameters of nandsim module can be found at `drivers/mtd/nand/nandsim.c`.
sudo modprobe nandsim first_id_byte=0x98 second_id_byte=0xd1 third_id_byte=0x90 fourth_id_byte=0x15 parts=256,256

# Get MTD info.
cat /proc/mtd
mtdinfo --all

# Attach `/dev/mtd1` and `/dev/mtd2` to UBI (create UBI device `/dev/ubi0` and `/dev/ubi1`).
# We can also use ubiattach which uses `/dev/ubi_ctrl` file.
sudo modprobe ubi mtd=1 mtd=2

# Create a ubi volume in `/dev/ubi0`.
sudo ubimkvol /dev/ubi0 -N rootfs -s 16MiB

# Mount ubifs on rootfs volume.
mkdir /tmp/ubifs
sudo mount -t ubifs ubi0:rootfs /tmp/ubifs
sudo touch /tmp/ubifs/README.TXT

sudo umount /tmp/ubifs
rmdir /tmp/ubifs

sudo rmmod ubi
sudo rmmod nandsim
```
