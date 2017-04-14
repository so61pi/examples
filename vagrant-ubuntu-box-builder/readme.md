**1. Install Ubuntu**

- You can install Ubuntu by either of following ways:
    - Install Ubuntu from an ISO then add necessary packages.
        - If you just want a Ubuntu with a GUI, you can install Ubuntu server and GUI package.
    - [Use a base box and install GUI packages on it](https://bitbucket.org/snippets/so61pi/qKBpA).


**2. [InGuest] Clean up apt cache**

```
sudo apt-get clean
```


**3. [InGuest] Add user `vagrant` to `sudo` group**

```
sudo usermod -aG sudo vagrant
```


**4. Pack the box to `.box` file**

```
# If you use a base box.
vagrant package -h
vagrant package --output /tmp/ubuntu.box --vagrantfile Vagrantfile
```

```
# If you install everything from an ISO.
vagrant package -h
vagrant package --base ubuntu --output /tmp/ubuntu.box --vagrantfile Vagrantfile
```


**5. Add the newly created box to current system**

```
vagrant box add /tmp/ubuntu.box --name ubuntu
```


**6. Use newly added box**

```
vagrant init ubuntu
vagrant up
```
