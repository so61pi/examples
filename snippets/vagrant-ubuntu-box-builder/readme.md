## Install Ubuntu

- You can install Ubuntu by either of following ways:
  - Install Ubuntu from an ISO then add necessary packages.
    - If you just want a Ubuntu with a GUI, you can install Ubuntu server and GUI package.
  - [Use a base box and install GUI packages on it](https://bitbucket.org/snippets/so61pi/qKBpA).

## [InGuest] Clean up apt cache

```shell
sudo apt-get clean
```

## [InGuest] Add user `vagrant` to `sudo` group

```shell
sudo usermod -aG sudo vagrant
```

## Pack the box to `.box` file

```shell
# If you use a base box.
vagrant package -h
vagrant package --output /tmp/ubuntu.box --vagrantfile Vagrantfile
```

```shell
# If you install everything from an ISO.
vagrant package -h
vagrant package --base ubuntu --output /tmp/ubuntu.box --vagrantfile Vagrantfile
```

## Add the newly created box to current system

```shell
vagrant box add /tmp/ubuntu.box --name ubuntu
```

## Use newly added box

```shell
vagrant init ubuntu
vagrant up
```
