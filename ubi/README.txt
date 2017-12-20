fallocate -l 1M rootfs.img
mkfs.ext2 -F rootfs.img
ubinize -o disk.ubi -m 512 -p 16KiB ubi.ini

# http://www.linux-mtd.infradead.org/doc/ubi.html
# http://free-electrons.com/blog/managing-flash-storage-with-linux/
