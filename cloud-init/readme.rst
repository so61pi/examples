.. contents:: Table of Contents

Softwares
=========

- CloudInit

  +-----------------+----------------------------------------------------------------------------------------+
  | Version         | 19.2                                                                                   |
  +-----------------+----------------------------------------------------------------------------------------+
  | Hash            | 060b1a1ca7b2385aa7f4ed42720063fa557e0671                                               |
  +-----------------+----------------------------------------------------------------------------------------+
  | Source Location | https://github.com/cloud-init/cloud-init/tree/060b1a1ca7b2385aa7f4ed42720063fa557e0671 |
  +-----------------+----------------------------------------------------------------------------------------+
  | Documentation   | https://cloudinit.readthedocs.io/en/19.2/index.html                                    |
  +-----------------+----------------------------------------------------------------------------------------+

Usage
=====

Cloud providers use cloud-init to quickly setup a machine with custom settings. The procedure is usually as follows:

#. Create a base image of a distro (e.g. Ubuntu, OpenSUSE).
#. Install cloud-init to that base image with correct configuration for that particular cloud provider.
   The configuration in this step applies to all future instances of this image.
#. When users choose to boot up from a base image, the cloud-init will be started in the boot process and find the correct configurations.
   Because each machine instance has different settings (e.g. hostname), cloud provider must provide a way to retrieve them.
   For example, EC2 exposes them at link-local address ``http://169.254.169.254``.

Command Line
============

``cloud-init init``
-------------------

This command's entry is at `cloud-init/cloudinit/cmd/main.py::main_init <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/cmd/main.py#L214>`__

    Cloud-init 'init' stage is broken up into the following sub-stages

    1. Ensure that the init object fetches its config without errors
    2. Setup logging/output redirections with resultant config (if any)
    3. Initialize the cloud-init filesystem
    4. Check if we can stop early by looking for various files
    5. **Fetch the datasource**

       - Each cloud vendor has a different way to provide the datasource, so they have separate handler. Handler for EC2 starts at `cloud-init/cloudinit/sources/DataSourceEc2.py::DataSourceEc2._get_data <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/sources/DataSourceEc2.py#L76>`__ which downloads relevant data from ``http://169.254.169.254``.

    6. Connect to the current instance location + update the cache
    7. **Consume the userdata** (handlers get activated here)
    8. Construct the modules object
    9. Adjust any subsequent logging/output redirections using the modules objects config as it may be different from init object
    10. **Run the modules for the 'init' stage**

        - ``cloud_init_modules``

    11. Done!

``cloud-init modules --mode=<init,config,final>``
-------------------------------------------------

This command's entry is at `cloud-init/cloudinit/cmd/main.py::main_modules <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/cmd/main.py#L467>`__

    Cloud-init 'modules' stages are broken up into the following sub-stages

    1. Ensure that the init object fetches its config without errors
    2. Get the datasource from the init object, if it does not exist then that means the main_init stage never worked, and thus this stage can not run.
    3. Construct the modules object
    4. Adjust any subsequent logging/output redirections using the modules objects configuration
    5. **Run the modules for the given stage name**

       - ``cloud_init_modules``
       - ``cloud_config_modules``
       - ``cloud_final_modules``

    6. Done!

References
----------

- https://cloudinit.readthedocs.io/en/19.2/topics/capabilities.html#cli-interface

Quick Look At Source Code
=========================

- Each module controls a few keywords. `Users and Groups module <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/config/cc_users_groups.py>`__ controls ``users`` and ``groups`` keywords, for instance.

- Some default configurations reside at `cloud-init/cloudinit/settings.py <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/settings.py>`__.

  * Default configuration location is ``/etc/cloud/cloud.cfg``.
  * Beside from ``/etc/cloud/cloud.cfg``, cloudinit also loads additional configurations in ``/etc/cloud/cloud.cfg.d/`` directory. The function responsible for loading and merging configurations is `cloud-init/cloudinit/stages.py::fetch_base_config <https://github.com/cloud-init/cloud-init/blob/060b1a1ca7b2385aa7f4ed42720063fa557e0671/cloudinit/stages.py#L878>`__.


``/etc/cloud/cloud.cfg`` Example
================================

.. code-block:: yml

    # The top level settings are used as module
    # and system configuration.

    # A set of users which may be applied and/or used by various modules
    # when a 'default' entry is found it will reference the 'default_user'
    # from the distro configuration specified below
    users:
       - default

    # If this is set, 'root' will not be able to ssh in and they
    # will get a message to login instead as the default $user
    disable_root: true

    # This will cause the set+update hostname module to not operate (if true)
    preserve_hostname: false

    # Example datasource config
    # datasource:
    #    Ec2:
    #      metadata_urls: [ 'blah.com' ]
    #      timeout: 5 # (defaults to 50 seconds)
    #      max_wait: 10 # (defaults to 120 seconds)

    # The modules that run in the 'init' stage
    cloud_init_modules:
     - migrator
     - seed_random
     - bootcmd
     - write-files
     - growpart
     - resizefs
     - disk_setup
     - mounts
     - set_hostname
     - update_hostname
     - update_etc_hosts
     - ca-certs
     - rsyslog
     - users-groups
     - ssh

    # The modules that run in the 'config' stage
    cloud_config_modules:
    # Emit the cloud config ready event
    # this can be used by upstart jobs for 'start on cloud-config'.
     - emit_upstart
     - snap
     - snap_config  # DEPRECATED- Drop in version 18.2
     - ssh-import-id
     - locale
     - set-passwords
     - grub-dpkg
     - apt-pipelining
     - apt-configure
     - ubuntu-advantage
     - ntp
     - timezone
     - disable-ec2-metadata
     - runcmd
     - byobu

    # The modules that run in the 'final' stage
    cloud_final_modules:
     - snappy  # DEPRECATED- Drop in version 18.2
     - package-update-upgrade-install
     - fan
     - landscape
     - lxd
     - ubuntu-drivers
     - puppet
     - chef
     - mcollective
     - salt-minion
     - rightscale_userdata
     - scripts-vendor
     - scripts-per-once
     - scripts-per-boot
     - scripts-per-instance
     - scripts-user
     - ssh-authkey-fingerprints
     - keys-to-console
     - phone-home
     - final-message
     - power-state-change

    # System and/or distro specific settings
    # (not accessible to handlers/transforms)
    system_info:
       # This will affect which distro class gets used
       distro: ubuntu
       # Default user name + that default users groups (if added/used)
       default_user:
         name: ubuntu
         lock_passwd: True
         gecos: Ubuntu
         groups: [adm, audio, cdrom, dialout, dip, floppy, lxd, netdev, plugdev, sudo, video]
         sudo: ["ALL=(ALL) NOPASSWD:ALL"]
         shell: /bin/bash
       # Automatically discover the best ntp_client
       ntp_client: auto
       # Other config here will be given to the distro class and/or path classes
       paths:
          cloud_dir: /var/lib/cloud/
          templates_dir: /etc/cloud/templates/
          upstart_dir: /etc/init/
       package_mirrors:
         - arches: [i386, amd64]
           failsafe:
             primary: http://archive.ubuntu.com/ubuntu
             security: http://security.ubuntu.com/ubuntu
           search:
             primary:
               - http://%(ec2_region)s.ec2.archive.ubuntu.com/ubuntu/
               - http://%(availability_zone)s.clouds.archive.ubuntu.com/ubuntu/
               - http://%(region)s.clouds.archive.ubuntu.com/ubuntu/
             security: []
         - arches: [arm64, armel, armhf]
           failsafe:
             primary: http://ports.ubuntu.com/ubuntu-ports
             security: http://ports.ubuntu.com/ubuntu-ports
           search:
             primary:
               - http://%(ec2_region)s.ec2.ports.ubuntu.com/ubuntu-ports/
               - http://%(availability_zone)s.clouds.ports.ubuntu.com/ubuntu-ports/
               - http://%(region)s.clouds.ports.ubuntu.com/ubuntu-ports/
             security: []
         - arches: [default]
           failsafe:
             primary: http://ports.ubuntu.com/ubuntu-ports
             security: http://ports.ubuntu.com/ubuntu-ports
       ssh_svcname: ssh

Ubuntu Cloud Image With KVM
===========================

Ubuntu cloud image comes with cloud-init pre-installed, we can use it with customized configuration to practice.

#. Go to https://cloud-images.ubuntu.com/ and download an image.

   After downloading, we can check its info by ``qemu-img info bionic-server-cloudimg-amd64.img``. The output should look like this::

       image: bionic-server-cloudimg-amd64.img
       file format: qcow2
       virtual size: 2.2 GiB (2361393152 bytes)
       disk size: 329 MiB
       cluster_size: 65536
       Format specific information:
           compat: 0.10
           refcount bits: 16

#. Prepare test image.

   If the downloaded image is not in qcow2 format, we can run the following command to do the conversion:

   .. code-block:: shell

       qemu-img convert -O qcow2 bionic-server-cloudimg-amd64.img testvm.qcow2

   Increase the size of the image by 10G (or whatever size you want):

   .. code-block:: shell

       qemu-img resize testvm.qcow2 +10G

   Move prepared image to proper location:

   .. code-block:: shell

       sudo mv testvm.qcow2 /var/lib/libvirt/images/

#. Prepare cloud-init config ISO:

   .. code-block:: shell

       mkisofs -volid cidata -joliet -rock -output testvm-cidata.iso user-data meta-data
       sudo mv testvm-cidata.iso /var/lib/libvirt/images/

#. Create new virtual machine.

   We can create new virtual machine from drafted files via Virtual Machine Manager (GUI) or virt-install (CLI).

   .. code-block:: shell

       # Remember to replace with suitable configs.
       sudo virt-install \
           --name testvm \
           --memory 2048 \
           --disk /var/lib/libvirt/images/testvm.qcow2,device=disk,bus=virtio \
           --disk /var/lib/libvirt/images/testvm-cidata.iso,device=cdrom \
           --os-type linux \
           --os-variant ubuntu18.04 \
           --virt-type kvm \
           --graphics none \
           --network network=default,model=virtio \
           --import

References
==========

- https://cloudinit.readthedocs.io/en/19.2/topics/instancedata.html#instance-metadata
- https://cloudinit.readthedocs.io/en/19.2/topics/datasources.html
- https://cloudinit.readthedocs.io/en/19.2/topics/examples.html
- https://github.com/cloud-init/cloud-init/tree/060b1a1ca7b2385aa7f4ed42720063fa557e0671/doc/examples
- https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/user-data.html
- https://wiki.archlinux.org/index.php/Cloud-init#Systemd_integration
