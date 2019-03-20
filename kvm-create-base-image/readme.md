## Install base OS

- Install ubuntu 16.04 in virt-manager with the name of `ubuntu-16.04-base`.

## Create base image

- Dump XML from base image

```shell
virsh dumpxml ubuntu-16.04-base > /images/ubuntu-16.04-base.xml
```

- Create template XML from the dumped XML
  - Remove `<mac>` address of network interface.
  - Remove machine's `<uuid>`.
  - Change driver type for future kvm instance from `raw` to `qcow2`.
    - From: `<driver name='qemu' type='raw'/>`
    - To: `<driver name='qemu' type='qcow2'/>`
  - Change machine's `<name>`.
    - From: `<name>ubuntu-16.04-base</name>`
    - To: `<name>your-virtual-machine-name</name>`
    - **You need to edit this field when create new virtual machine instance**.
  - Change source image file for future kvm instance.
    - From: `<source file='/var/lib/libvirt/images/ubuntu-16.04-base.img'/>`
    - To: `<source file='/path/to/your/image.qcow2'/>`
    - **You need to edit this field when create new virtual machine instance**.
  - You can make other changes like add new hardware, for more information about the XML format, please read [https://libvirt.org/formatdomain.html](https://libvirt.org/formatdomain.html).

## Save base image

```shell
sudo cp /var/lib/libvirt/images/ubuntu-16.04-base.img /images/
```

## Strip base image

```shell
sudo virt-sysprep -a /images/ubuntu-16.04-base.img
# on Ubuntu, you need to install libguestfs-tools to make virt-sysprep available
# sudo apt-get install libguestfs-tools
# update-guestfs-appliance
```

## Deploy new instance

- Create qcow2 image for new instance base on base image

```shell
cd /var/lib/libvirt/images/
qemu-img create -f qcow2 -b /images/ubuntu-16.04-base.img ubuntu-test.qcow2
```

- Create a new XML for new instance

```shell
cp /images/ubuntu-16.04-base.xml /tmp/ubuntu-test.xml
# change <name> and <source file>
# make other changes if you need
```

- Start it

```shell
virsh create /tmp/ubuntu-test.xml
```
