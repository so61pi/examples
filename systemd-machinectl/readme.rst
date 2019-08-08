.. contents:: Table of Contents

Creating ``rootfs``
===================

Before running commands in this section, ``mkosi`` package should already be installed.

Configuration File
------------------

``mkosi.default``
~~~~~~~~~~~~~~~~~

Raw Image With GPT
``````````````````

.. code-block:: ini

    [Distribution]
    Distribution=opensuse
    Release=15.1

    [Output]
    Format=raw_gpt
    Bootable=yes
    Output=opensuse-leap.raw

    [Packages]
    Packages=ca-certificates curl git iproute2 iputils sudo vim

    [Validation]
    Password=root

btrfs subvolume
```````````````

.. code-block:: ini

    [Distribution]
    Distribution=opensuse
    Release=15.1

    [Output]
    Format=subvolume
    Output=opensuse-leap-template

    [Packages]
    Packages=ca-certificates curl git iproute2 iputils sudo vim

    [Validation]
    Password=root

Other Useful Options
````````````````````

- Set ``rootfs`` to read-only

  * Adding ``ReadOnly=true`` to ``[Output]`` section.

- Enable compression

  * Adding ``Compress=true`` to ``[Output]`` section.

- Output to ``QEMU`` image format

  * Adding ``QCow2=true`` to ``[Output]`` section.

``mkosi.postinst``
~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    #!/bin/sh

    # Generate hash password by
    #     python3 -c 'import crypt; print(crypt.crypt("opensuse", crypt.mksalt(crypt.METHOD_SHA512)))'
    useradd -g users -m -s /bin/bash -u 1000 -p '$6$KOQkMxMqSDppB2QB$Tl3usIqM.lymKexE0d33cnZbKyOCCSzENiTU5afYfzE8U2lkHmp3BgTx.Wxql6GBREDTd1PEVDz3YXREQgHQa/' opensuse

Don't forget to make this file executable.

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

Booting With ``systemd-nspawn``
===============================

.. code-block:: shell

    # Boot a raw image
    sudo systemd-nspawn -b -i opensuse-leap.raw

    # Boot from a rootfs directory
    sudo systemd-nspawn -b -D opensuse-leap-template

    # Run container in a temporary snapshot, remove it after shutting down
    sudo systemd-nspawn -b -D opensuse-leap-template --ephemeral

    # Snapshot template to a new directory, then boot from that
    sudo systemd-nspawn -b -D opensuse-leap --template=opensuse-leap-template

To mount a directory or file from host, we add ``--bind=<host-path>:<container-path>`` option to ``systemd-nspawn``.

*Note*

- We could put the command line options to a ``.nspawn`` file, but due to the way ``systemd-nspawn`` treat privilege settings in ``.nspawn`` file base on its location, we should just run ``systemd-nspawn`` directly.
- In case we want to use docker in the machine, we can:

  * Disable ``docker.socket`` unit by ``sudo systemctl mask docker.socket``.
  * Install docker.
  * Create a group called ``dockerx`` with GID same as ``docker`` group of host (``sudo groupadd -g <GID> dockerx``).
  * Add ``$USER`` to group ``dockerx`` (``sudo usermod -aG dockerx $USER``) so we can run ``docker`` command without ``sudo``.
  * Mount host's ``/var/run/docker.sock``.
  * **Note** that docker inside the machine will talk to docker daemon of host, which increases security risk.

References
----------

- https://www.freedesktop.org/software/systemd/man/systemd-nspawn.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html#.nspawn%20File%20Discovery
- https://jpetazzo.github.io/2015/09/03/do-not-use-docker-in-docker-for-ci/

Booting With ``machinectl``
===========================

``machinectl`` uses template unit ``systemd-nspawn@.service`` for creating containers.

Importing ``rootfs``
--------------------

.. code-block:: shell

    # Copying a directory to /var/lib/machines/
    sudo machinectl import-fs <directory> <machine-name>

    # Copying a file to /var/lib/machines/
    sudo machinectl import-raw <file> <machine-name>

    # Extracting a file to a directory /var/lib/machines/
    sudo machinectl import-tar <file> <machine-name>

Starting Container
------------------

.. code-block:: shell

    # Starting a machine, this uses systemd-nspawn underneath
    sudo machinectl start <machine-name>

    # Open a login shell to a machine
    sudo machinectl login <machine-name>

Stopping Container
------------------

.. code-block:: shell

    sudo machinectl stop <machine-name>

Enabling/Disabling Container To Start At Startup
------------------------------------------------

.. code-block:: shell

    sudo machinectl enable <machine-name>
    sudo machinectl disable <machine-name>

Mounting Host's Directory/File Into Container
---------------------------------------------

.. code-block:: shell

    sudo machinectl --mkdir bind <machine-name> <host-path> <container-path>

Networking
----------

The default template unit ``systemd-nspawn@.service`` uses ``--network-veth`` option which isolates container's network from host's network. Hence, the container cannot connect to the outside world.

A simple way to overcome this is to create an override file for container's unit with new ``systemd-nspawn`` command. However, if we edit ``systemd-nspawn@.service`` directly, it will affect all containers, old and new.

Therefore, we will edit container's specific unit.

.. code-block:: shell

    sudo systemctl edit systemd-nspawn@<machine-name>.service

And here the content of the override file:

.. code-block:: ini

    # cat /usr/lib/systemd/system/systemd-nspawn@.service
    #
    # Empty ExecStart= is to reset ExecStart list.
    # https://www.freedesktop.org/software/systemd/man/systemd.service.html#ExecStart=
    [Service]
    ExecStart=
    ExecStart=/usr/bin/systemd-nspawn --quiet --keep-unit --boot --link-journal=try-guest --settings=override --machine=%i

To remove the override file, use ``sudo systemctl revert systemd-nspawn@<machine-name>.service``.

DNS
~~~

You might want to check the DNS settings in case domains cannot be resolved. To do so, we have to edit ``/etc/systemd/resolved.conf`` then run ``sudo systemctl status systemd-resolved`` to reload settings.

Additional Options
------------------

We could specify additional options for ``systemd-nspawn`` in a ``.nspawn`` file and place it at ``/etc/systemd/nspawn/<machine-name>.nspawn``.

Example ``/etc/systemd/nspawn/opensuse-leap.nspawn``:

.. code-block:: ini

    # https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html
    [Network]
    Private=no
    VirtualEthernet=no

References
----------

- https://www.freedesktop.org/software/systemd/man/machinectl.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html
- https://www.freedesktop.org/software/systemd/man/systemd.nspawn.html#.nspawn%20File%20Discovery
