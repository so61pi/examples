# Compile linux kernel

- create default configuration
- make changes if needed
- make, the output kernel file's location will be printed on the console

```bash
make defconfig
make menuconfig
    # 64-bit kernel
    # Kernel hacking -> Compile-time check and compiler options -> Compile the kernel with debug info
make
```


# Compile busybox

- for simplicity, we don't use shared libs
- to get smaller binary files, you can compile with `uClibc`
- after make install, all necessary files are in `_install`

```bash
make menuconfig
    # Busybox Settings -> Build Options -> Build BusyBox as a static library (no shared libs)
make
make install
```


# Compile qemu

- before compile qemu, please make sure you have all required libs
- you can just compile for some appropriate architectures by using `--target-list`, this also reduces compile time

```bash
./configure --prefix=$HOME/work/tools/qemu --enable-sdl
make
make install
```


# Create init file

- below is the init file's content

```bash
#!/bin/sh

# enable /dev dir
mount -t devtmpfs none /dev

# mount essential dirs
[ -d /sys ]  || mkdir /sys
[ -d /proc ] || mkdir /proc
[ -d /tmp ]  || mkdir /tmp
mount -t sysfs sysfs /sys
mount -t proc proc /proc
mount -t tmpfs tmpfs /tmp

if [ -e /dev/sda ]
then
    mount -t ext4 /dev/sda /root
    mount --move /proc /root/proc
    mount --move /sys /root/sys
    mount --move /dev /root/dev
    exec switch_root /root /sbin/init
else
    echo "Cannot find /dev/sda"
    echo "Note: Don't try to switch_root here"
    /bin/sh
fi
```


# Create initramfs

- you could use `mkramfs.sh`

```bash
mkdir initramfs
cp busybox/* initramfs
cp init initramfs
chmod 777 initramfs/init
cd initramfs
find . | cpio -o --format=newc > initramfs.cpio
```


# Create disk image

- create a disk image in ext4 format
- copy your system files into that image
- here we just use busybox files & directories for the sake of simplicity
- you could use `mkdisk.sh`

```bash
fallocate -l 50M disk.img
mkfs.ext4 -F disk.img
mkdir /mnt/disk
mount -t ext4 disk.img /mnt/disk
cp busybox/* /mnt/disk
[ -d /mnt/disk/etc  ] || mkdir /mnt/disk/etc
[ -d /mnt/disk/dev  ] || mkdir /mnt/disk/dev
[ -d /mnt/disk/proc ] || mkdir /mnt/disk/proc
[ -d /mnt/disk/sys  ] || mkdir /mnt/disk/sys
[ -d /mnt/disk/tmp  ] || mkdir /mnt/disk/tmp
chown -R root:root /mnt/disk
umount /mnt/disk
```


# Run

- when running qemu with `-nographic` option, use `Ctrl+A H` to get help
- in case the kernel doesn't boot, you should increase RAM size with `-m` option (default value is 128M)

```bash
qemu-system-x86_64 -kernel bzImage -initrd initramfs.cpio -nographic -append "console=ttyS0"
qemu-system-x86_64 -kernel bzImage -initrd initramfs.cpio -hda rootfs.img -nographic -append "console=ttyS0"
```


# Note

- if you try to boot to ramfs, start busybox's `/bin/sh`, mount `/dev/sda` to `/root` and `switch_root` all by yourself, then, DON'T do it, or you will get an error

```
Kernel panic - not syncing - Attempted to kill init!
```

- you can consider using `klibc`


# Documents

- initramfs
    - https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt
    - http://stackoverflow.com/questions/10603104/the-difference-between-initrd-and-initramfs
- grub
    - https://www.gnu.org/software/grub/manual/grub.html
        - Commands
            - linux
                - load linux kernel image (usually vmlinuz or vmlinux) from disk to RAM
            - initrd
                - load initial ramdisk, then set appropriate parameters in the linux setup area in memory
            - boot
                - actually boot linux
- x86 boot protocol
    - https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt
- x86 boot process
    - https://github.com/0xAX/linux-insides/tree/master/Booting
- linux source
    - arch/x86/boot/setup.ld
    - arch/x86/boot/header.S::_start
    - arch/x86/boot/main.c::void main(void)
    - init/main.c::void start_kernel(void)
