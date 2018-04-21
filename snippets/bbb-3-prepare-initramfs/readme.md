## Preparation

- Assume `/tmp/initramfs/` already has busybox.

## Completing rootfs

```shell
cd /tmp/initramfs
mkdir dev
chmod 755 dev
mknod -m 600 dev/console c 5 1

mkdir root
chmod 700 root
cd -
```

## Create `/init` file

- Below is the content of `/tmp/initramfs/init` file.

```shell
#!/bin/sh

mount -t devtmpfs none /dev

[ -d /sys ]  || mkdir /sys
[ -d /proc ] || mkdir /proc
[ -d /tmp ]  || mkdir /tmp
mount -t sysfs sysfs /sys
mount -t proc proc /proc
mount -t tmpfs tmpfs /tmp

/bin/sh
```

- Make it executable.

```shell
chmod +x /tmp/initramfs/init
```

## Create `initramfs.cpio`

```shell
cd /tmp/initramfs
find . | cpio -o --format=newc > initramfs.cpio
```

## Create `initramfs.uImage`

- If we want to boot with initramfs in uImage, we can create `initramfs.uImage` with the following command.

```shell
mkimage -A arm -O linux -T ramdisk -C none -d initramfs.cpio initramfs.uImage
```
