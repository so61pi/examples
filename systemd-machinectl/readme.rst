.. contents:: Table of Contents

Creating ``rootfs``
===================

Configuration File
------------------

Raw Image With GPT
~~~~~~~~~~~~~~~~~~

``mkosi.default``::

    [Distribution]
    Distribution=opensuse
    Release=15.1

    [Output]
    Format=raw_gpt
    Bootable=yes
    Output=opensuse-leap.raw

    [Packages]
    Packages=git iproute2 iputils vim

    [Validation]
    Password=root

btrfs subvolume
~~~~~~~~~~~~~~~

``mkosi.default``::

    [Distribution]
    Distribution=opensuse
    Release=15.1

    [Output]
    Format=subvolume
    Output=opensuse-leap-template

    [Packages]
    Packages=git iproute2 iputils vim

    [Validation]
    Password=root

Other Useful Options
~~~~~~~~~~~~~~~~~~~~

- Set ``rootfs`` to read-only

  * Adding ``ReadOnly=true`` to ``[Output]`` section.

- Enable compression

  * Adding ``Compress=true`` to ``[Output]`` section.

- Output to ``QEMU`` image format

  * Adding ``QCow2=true`` to ``[Output]`` section.

Building ``rootfs``
-------------------

- Build the ``rootfs``

  .. code-block:: shell

      mkdir -p mkosi.cache
      sudo mkosi build

- Clean the built artifacts

  .. code-block:: shell

      # Remove artifacts of previous build
      sudo mkosi clean

      # Remove incremental build cache
      sudo mkosi -f clean

      # Remove all cache
      sudo mkosi -f -f clean

References
----------

- https://github.com/systemd/mkosi/blob/master/mkosi.md
- https://wiki.archlinux.org/index.php/Mkosi

Booting with ``systemd-nspawn``
===============================

.. code-block:: shell

    # Boot a raw image
    sudo systemd-nspawn -b -U -i opensuse-leap.raw

    # Boot from a rootfs directory
    sudo systemd-nspawn -b -U -D opensuse-leap-template

    # Run container in a temporary snapshot, remove it after shutting down
    sudo systemd-nspawn -b -U -D opensuse-leap-template --ephemeral

    # Snapshot template to a new directory, then boot from that
    sudo systemd-nspawn -b -U -D opensuse-leap --template=opensuse-leap-template

*Note*: We could put the command line options to a ``.nspawn`` file, but due to the way ``systemd-nspawn`` treat privilege settings in ``.nspawn`` file base on its location, we should just run ``systemd-nspawn`` directly.

References
----------

- https://www.freedesktop.org/software/systemd/man/systemd-nspawn.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html#.nspawn%20File%20Discovery

Booting with ``machinectl``
===========================

``machinectl`` uses template unit ``systemd-nspawn@.service`` for creating containers.

Importing ``rootfs``
--------------------

.. code-block:: shell

    # Copying a directory to /var/lib/machines/
    machinectl import-fs <directory> <machine-name>

    # Copying a file to /var/lib/machines/
    machinectl import-raw <file> <machine-name>

    # Extracting a file to a directory /var/lib/machines/
    machinectl import-tar <file> <machine-name>

Starting Container
------------------

.. code-block:: shell

    # Starting a machine, this uses systemd-nspawn underneath
    machinectl start <machine-name>

    # Open a login shell to a machine
    machinectl login <machine-name>

Stopping Container
------------------

.. code-block:: shell

    machinectl stop <machine-name>

Enabling/Disabling Container To Start At Startup
------------------------------------------------

.. code-block:: shell

    machinectl enable <machine-name>
    machinectl disable <machine-name>

Networking
----------

The default template unit ``systemd-nspawn@.service`` uses ``--network-veth`` option which isolates container's network from host's network. Hence, the container cannot connect to the outside world.

A simple way to overcome this is to create an override file for container's unit with new ``systemd-nspawn`` command. However, if we edit ``systemd-nspawn@.service`` directly, it will affect all containers, old and new.

Therefore, we will edit container's specific unit.

.. code-block:: shell

    sudo systemctl edit systemd-nspawn@<machine-name>.service

And here the content of the override file::

    [Service]
    # Empty ExecStart= is to reset ExecStart list.
    # https://www.freedesktop.org/software/systemd/man/systemd.service.html#ExecStart=
    ExecStart=

    ExecStart=/usr/bin/systemd-nspawn --quiet --keep-unit --boot --link-journal=try-guest -U --settings=override --machine=%i

Additional Options
------------------

We could specify additional options for ``systemd-nspawn`` in a ``.nspawn`` file and place it at ``/etc/systemd/nspawn/<machine-name>.nspawn``.

Example ``/etc/systemd/nspawn/opensuse-leap.nspawn``::

    # https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html

    [Network]
    Private=no
    VirtualEthernet=no

References
----------

- https://www.freedesktop.org/software/systemd/man/machinectl.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html#.nspawn%20File%20Discovery
