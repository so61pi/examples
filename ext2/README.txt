fallocate -l 5M disk.img
mkfs.ext2 -F disk.img
mkdir /mnt/disk
mount -t ext2 disk.img /mnt/disk
cp hello.txt /mnt/disk
mkdir /mnt/disk/mydir
cp main.cpp /mnt/disk/mydir
chown -R root:root /mnt/disk
umount /mnt/disk


http://www.nongnu.org/ext2-doc/ext2.html