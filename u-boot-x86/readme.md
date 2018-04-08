**softwares**

- Linux `v4.16` (`0adb32858b0bddf4ada5f364a84ed60b196dbcda`)
- U-Boot  `v2018.03` (`f95ab1fb6e37f0601f397091bb011edf7a98b890`)


**make**

- Linux

```
make defconfig
make
cp arch/x86/boot/bzImage /tftpboot
```

- U-Boot

```
make qemu-x86_defconfig
make
```


**qemu**

```shell
# We don't even need to install another TFTP on our machine as qemu already supports an internal one
qemu-system-x86_64 -nographic -device e1000,netdev=n0 -netdev user,id=n0,tftp=/tftpboot -bios u-boot.rom
```

**u-boot**

- `dhcp` automatically sets `serverip`
- `tftp` automatically uses `loadaddr` and `bootfile` and set `fileaddr`
- `zboot` automatically uses `fileaddr`

```shell
setenv autoload no
dhcp
tftp

setenv bootargs console=$consoledev,$baudrate
zboot
```


**env**

- `printenv` before `tftp`

```shell
baudrate=115200
bootargs=root=/dev/sdb3 init=/sbin/init rootwait ro
bootcmd=ext2load scsi 0:3 01000000 /boot/vmlinuz; zboot 01000000
bootdelay=2
bootfile=bzImage
consoledev=ttyS0
ethaddr=52:54:00:12:34:56
fdtcontroladdr=7d55440
hostname=x86
loadaddr=0x1000000
netdev=eth0
nfsboot=setenv bootargs root=/dev/nfs rw nfsroot=$serverip:$rootpath ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=$consoledev,$baudrate $othbootargs;tftpboot $loadaddr $bootfile;zboot $loadaddr
pciconfighost=1
ramboot=setenv bootargs root=/dev/ram rw ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=$consoledev,$baudrate $othbootargs;tftpboot $loadaddr $bootfile;tftpboot $ramdiskaddr $ramdiskfile;zboot $loadaddr 0 $ramdiskaddr $filesize
ramdiskaddr=0x4000000
ramdiskfile=initramfs.gz
rootpath=/opt/nfsroot
stderr=serial,vidconsole
stdin=serial,i8042-kbd
stdout=serial,vidconsole
```

- `printenv` after `tftp`

```shell
autoload=no
baudrate=115200
bootargs=root=/dev/sdb3 init=/sbin/init rootwait ro
bootcmd=ext2load scsi 0:3 01000000 /boot/vmlinuz; zboot 01000000
bootdelay=2
bootfile=bzImage
consoledev=ttyS0
dnsip=10.0.2.3
ethact=e1000#0
ethaddr=52:54:00:12:34:56
fdtcontroladdr=7d55440
fileaddr=1000000
filesize=7a05b0
gatewayip=10.0.2.2
hostname=x86
ipaddr=10.0.2.15
loadaddr=0x1000000
netdev=eth0
netmask=255.255.255.0
nfsboot=setenv bootargs root=/dev/nfs rw nfsroot=$serverip:$rootpath ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=$consoledev,$baudrate $othbootargs;tftpboot $loadaddr $bootfile;zboot $loadaddr
pciconfighost=1
ramboot=setenv bootargs root=/dev/ram rw ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=$consoledev,$baudrate $othbootargs;tftpboot $loadaddr $bootfile;tftpboot $ramdiskaddr $ramdiskfile;zboot $loadaddr 0 $ramdiskaddr $filesize
ramdiskaddr=0x4000000
ramdiskfile=initramfs.gz
rootpath=/opt/nfsroot
serverip=10.0.2.2
stderr=serial,vidconsole
stdin=serial,i8042-kbd
stdout=serial,vidconsole
```
