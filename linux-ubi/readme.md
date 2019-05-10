## Acronyms

- **EC**: Erase Counter
- **LEB**: Logical Erase Block
- **MTD**: Memory Technology Devices
- **PEB**: Physical Erase Block
- **UBI**: Unsorted Block Images
- **VID**: Volume Identifier

## Overview

- "**MTD subsystem (stands for Memory Technology Devices) provides an abstraction layer for raw flash devices.** It makes it possible to use the same API when working with different flash types and technologies, e.g. NAND, OneNAND, NOR, AG-AND, ECC'd NOR, etc."
- **`/dev/mtdX` presents an MTD partition (it's also called an MTD device).**
  - There is nothing like `/dev/mtd` for MTD devices (like the relationship between `/dev/sda` and `/dev/sda0`). If an MTD device has 3 partitions, they are `/dev/mtd0`, `/dev/mtd1` and `/dev/mtd2`, and that's it.
  - There is also no MTD partition table in the MTD devices, it's defined in the device tree or via kernel parameter `mtdparts`.
- **The UBI is "a volume management system for raw flash devices which manages multiple logical volumes on a single physical flash device and spreads the I/O load (i.e, wear-leveling) across whole flash chip".**
  - It uses MTD sub-system to interact (reading, writing...) with the underlying MTD device (partition).
  - **`/dev/ubiX` is called a UBI device**, it is created when we attach an MTD device (partition) to UBI.
  - UBI supports [read-only block devices](http://www.linux-mtd.infradead.org/faq/ubi.html#L_squashfs_over_ubi), but **not** [read-write block devices](http://www.linux-mtd.infradead.org/faq/ubi.html#L_ext2_over_ubi).

```txt
+-------------------------------------+
|           UBIFS, SQUASHFS           |
+-------------------------------------+
|             UBI volumes             |
|       ubi0:rootfs ubi0:config       |
|       ubi1:rootfs ubi1:config       |
+-------------------------------------+
|                 UBI                 |
|            (UBI devices)            |
|         /dev/ubi0 /dev/ubi1         |
+-------------------------------------+
|                 MTD                 |
|            (MTD devices)            |
|         /dev/mtd0 /dev/mtd1         |
+-------------------------------------+
|            Flash Device             |
+-------------------------------------+

```

## `nandsim`

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

## Flow

- Linux `v4.10.13`.

```txt
drivers/mtd/ubi/build.c::ubi_init
    drivers/mtd/ubi/build.c::open_mtd_device
    drivers/mtd/ubi/build.c::ubi_attach_mtd_dev
        drivers/mtd/ubi/attach.c::ubi_attach
            drivers/mtd/ubi/attach.c::scan_all
                drivers/mtd/ubi/attach.c::scan_peb
                    drivers/mtd/ubi/io.c::ubi_io_read_ec_hdr
                        drivers/mtd/drivers/mtd/ubi/io.c::ubi_io_read
                            drivers/mtd/mtdcore.c::mtd_read
                        drivers/mtd/ubi/io.c::validate_ec_hdr
                    drivers/mtd/ubi/io.c::ubi_io_read_vid_hdr
                        drivers/mtd/drivers/mtd/ubi/io.c::ubi_io_read
                            drivers/mtd/mtdcore.c::mtd_read
                        drivers/mtd/ubi/io.c::validate_vid_hdr
            drivers/mtd/ubi/vtbl.c::ubi_read_volume_table
            drivers/mtd/ubi/wl.c::ubi_wl_init
            drivers/mtd/ubi/eba.c::ubi_eba_init


drivers/mtd/ubi/io.c::ubi_io_write_ec_hdr
    drivers/mtd/ubi/io.c::ubi_io_write
        drivers/mtd/mtdcore.c::mtd_write


drivers/mtd/ubi/io.c::ubi_io_write_vid_hdr
    drivers/mtd/ubi/io.c::ubi_io_write
        drivers/mtd/mtdcore.c::mtd_write
```

## Create `disk.ubi`

```shell
fallocate -l 1M rootfs.img
mkfs.ext2 -F rootfs.img
ubinize -o disk.ubi -m 512 -p 16KiB ubi.ini
```

## References

- http://www.linux-mtd.infradead.org/doc/ubi.html
- http://www.linux-mtd.infradead.org/faq/general.html
- http://free-electrons.com/blog/managing-flash-storage-with-linux/
- [UBI Usage in U-Boot](http://www.denx.de/wiki/publish/DULG/DULG-enbw_cmc.html#Section_5.9.3.6.)
- drivers/mtd/nand/
- drivers/mtd/ubi/
