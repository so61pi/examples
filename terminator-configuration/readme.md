**softwares**

- Ubuntu 16.04.2


**font**


**scrolling**


**custom commands**

- add the below snippet to `~/.config/terminator/config`

```
[plugins]
  [[CustomCommandsMenu]]
    [[[~ cd HOME]]]
      command = cd ~
      enabled = True
      name = ~ cd HOME
      position = 0
    [[[~ cd work]]]
      command = cd ~/work/
      enabled = True
      name = ~ cd work
      position = 1
    [[[~ cd downloads]]]
      command = cd ~/work/downloads/
      enabled = True
      name = ~ cd downloads
      position = 2
    [[[~ cd programming]]]
      command = cd ~/work/programming/
      enabled = True
      name = ~ cd programming
      position = 3
    [[[~ cd projects]]]
      command = cd ~/work/programming/projects/
      enabled = True
      name = ~ cd projects
      position = 4
    [[[~ cd sources]]]
      command = cd ~/work/programming/sources/
      enabled = True
      name = ~ cd sources
      position = 5
    [[[~ cd tmp]]]
      command = cd ~/work/tmp/
      enabled = True
      name = ~ cd tmp
      position = 6
    [[[~ cd tools]]]
      command = cd ~/work/tools/
      enabled = True
      name = ~ cd tools
      position = 7
    [[[~ cd vms]]]
      command = cd ~/work/vms/
      enabled = True
      name = ~ cd vms
      position = 8
    [[[~ cd warehouse]]]
      command = cd ~/work/warehouse/
      enabled = True
      name = ~ cd warehouse
      position = 9
    [[[---------------------------------------- (1)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (1)
      position = 10
    [[[apt upgrade]]]
      command = sudo apt update && sudo apt upgrade
      enabled = True
      name = apt upgrade
      position = 11
    [[[apt update]]]
      command = sudo apt update
      enabled = True
      name = apt update
      position = 12
    [[[apt autoremove]]]
      command = sudo apt autoremove
      enabled = True
      name = apt autoremove
      position = 13
    [[[---------------------------------------- (2)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (2)
      position = 14
    [[[clear]]]
      command = clear
      enabled = True
      name = clear
      position = 15
    [[[reset]]]
      command = reset
      enabled = True
      name = reset
      position = 16
    [[[---------------------------------------- (3)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (3)
      position = 17
    [[[exit]]]
      command = exit
      enabled = True
      name = exit
      position = 18
```