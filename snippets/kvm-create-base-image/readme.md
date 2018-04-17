In this guide, we will put base image and template XML to `/images` directory, but you can put them to anywhere you like.

# 1. Install base OS
- Install ubuntu 16.04 in virt-manager with the name of `ubuntu-16.04-base`.


# 2. Create base image
1. Dump XML from base image

        virsh dumpxml ubuntu-16.04-base > /images/ubuntu-16.04-base.xml

2. Create template XML from the dumped XML
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

3. Save base image

        sudo cp /var/lib/libvirt/images/ubuntu-16.04-base.img /images/

4. Strip base image

        sudo virt-sysprep -a /images/ubuntu-16.04-base.img
        # on Ubuntu, you need to install libguestfs-tools to make virt-sysprep available
        # sudo apt-get install libguestfs-tools
        # update-guestfs-appliance


# 3. Deploy new instance
1. Create qcow2 image for new instance base on base image

        cd /var/lib/libvirt/images/
        qemu-img create -f qcow2 -b /images/ubuntu-16.04-base.img ubuntu-test.qcow2

2. Create a new XML for new instance

        cp /images/ubuntu-16.04-base.xml /tmp/ubuntu-test.xml
        # change <name> and <source file>
        # make other changes if you need

3. Start it

        virsh create /tmp/ubuntu-test.xml
