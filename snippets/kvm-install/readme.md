## Install KVM

```shell
sudo apt install qemu-system qemu-kvm libvirt-bin virt-manager python-spice-client-gtk
#sudo adduser `id -un` libvirtd
```

## Install remote viewer

```shell
sudo apt install remote-viewer
#sudo apt install spice-client
```

## Enable Spice

- Read
  - http://www.linux-kvm.org/page/SPICE
- Show virtual hardware details
  - Display: Spice
  - Video: QXL

## Connect to guest remotely

```shell
remote-viewer spice://host:port
#spicec -h host -p port -w password
```

## Install vdagent in guest to enable spice functionalities (shared clipboard...)

```shell
sudo apt update
sudo apt install spice-vdagent
```

## Enable file shared between host and guest

- Read
  - http://www.linux-kvm.org/page/9p_virtio
- Show virtual hardware details
  - Add Hardware
    - Filesystem
      - Source path: /tmp/share
      - Target path: /hostshare
- Login to guest

```shell
mkdir /tmp/host_files
sudo mount -t 9p -o trans=virtio,version=9p2000.L /hostshare /tmp/host_files
```

## Read

- KVM
  - [Documents](http://www.linux-kvm.org/page/Documents)
  - [Guest Support Status](http://www.linux-kvm.org/page/Guest_Support_Status)
  - [Management Tools](http://www.linux-kvm.org/page/Management_Tools)
  - Google("kvm architecture")
- Spice
  - [Spice for Newbies](http://www.spice-space.org/documentation.html)
  - [Spice User Manual](http://www.spice-space.org/static/docs/)
  - http://www.spice-space.org/page/Repositories
- libvirt
  - [FAQ](http://wiki.libvirt.org/page/FAQ)
  - [virsh command reference](http://libvirt.org/virshcmdref.html)

## Note

- If you have problem about read permission to access iso file when start installing VM, please make sure the upper dirs of the absolue path to iso file must have execute permission for other user (the iso file itself must have read permission of course).
  - Example: `/data/iso/ubuntu.iso`
    - `/data` must have permission at least like `dr-xr-xr-x` (maybe more like `drwxr-xr-x` or `drwxrwxr-x`).
    - `/data/iso` must have permission at least like `dr-xr-xr-x` (maybe more like `drwxr-xr-x` or `drwxrwxr-x`).
    - `/data/iso/ubuntu.iso` (the iso file) must have read permission for other users (like `drwxrwxr--`).
