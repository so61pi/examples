## Software

- Linux

## Reference

- Documentation/block/cmdline-partition.txt
- block/partitions/cmdline.c
- block/partitions/Kconfig

## Overview

- Linux has supports for `blkdevparts` which works similar to `mtdparts`. The block devices don't have to have the partition table and we can have more than 4 partitions per device. The downside is the partition layout is encoded in the kernel's boot commandline.
- The behavior is controled by `CONFIG_CMDLINE_PARTITION`.
