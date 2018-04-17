**1. Install Windows**

- We can remove unnescessary hardware components like floppy disk or audio.
- Username and password should both be `vagrant` to conform with `Vagrantfile`.


**2. [InGuest] Install VirtualBox Guest Additions**

- Remember to eject the disc after installation.


**3. [InGuest] Enable and configure WinRM**

```bat
:: Run in Administrative Command Prompt.
winrm quickconfig -q
winrm set winrm/config/winrs @{MaxMemoryPerShellMB="512"}
winrm set winrm/config @{MaxTimeoutms="1800000"}
winrm set winrm/config/service @{AllowUnencrypted="true"}
winrm set winrm/config/service/auth @{Basic="true"}
sc config WinRM start= auto
```


**4. [InGuest] Enable Remote Desktop**

```bat
:: Run in Administrative Command Prompt.
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f

:: Or you can google("windows enable remote desktop") to configure through the GUI.
```


**5. [InGuest] Remove unnecessary files**

```bat
:: Run in Administrative Command Prompt.
cleanmgr /d C:
```


**6. Pack the box to `.box` file**

```
vagrant package -h
vagrant package --base windows-7 --output /tmp/windows-7.box --vagrantfile Vagrantfile
```

*Note that the `Vagrantfile` here is different from the one when you run `vagrant init`, you should read about [Vagrantfile load order](https://www.vagrantup.com/docs/vagrantfile/#load-order).*


**7. Add the newly created box to current system**

```
vagrant box add /tmp/windows-7.box --name windows-7
```


**8. Use newly added box**

```
vagrant init windows-7
vagrant up
```
