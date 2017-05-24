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
    [[[~ cd work]]]
      command = cd ~/work/
      enabled = True
      name = ~ cd work
    [[[~ cd downloads]]]
      command = cd ~/work/downloads/
      enabled = True
      name = ~ cd downloads
    [[[~ cd programming]]]
      command = cd ~/work/programming/
      enabled = True
      name = ~ cd programming
    [[[~ cd projects]]]
      command = cd ~/work/programming/projects/
      enabled = True
      name = ~ cd projects
    [[[~ cd sources]]]
      command = cd ~/work/programming/sources/
      enabled = True
      name = ~ cd sources
    [[[~ cd tmp]]]
      command = cd ~/work/tmp/
      enabled = True
      name = ~ cd tmp
    [[[~ cd tools]]]
      command = cd ~/work/tools/
      enabled = True
      name = ~ cd tools
    [[[~ cd vms]]]
      command = cd ~/work/vms/
      enabled = True
      name = ~ cd vms
    [[[~ cd warehouse]]]
      command = cd ~/work/warehouse/
      enabled = True
      name = ~ cd warehouse
    [[[---------------------------------------- (1)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (1)
    [[[apt upgrade]]]
      command = sudo apt update && sudo apt upgrade
      enabled = True
      name = apt upgrade
    [[[apt update]]]
      command = sudo apt update
      enabled = True
      name = apt update
    [[[apt autoremove]]]
      command = sudo apt autoremove
      enabled = True
      name = apt autoremove
    [[[---------------------------------------- (2)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (2)
    [[[clear]]]
      command = clear
      enabled = True
      name = clear
    [[[reset]]]
      command = reset
      enabled = True
      name = reset
    [[[---------------------------------------- (3)]]]
      command = :
      enabled = True
      name = ---------------------------------------- (3)
    [[[exit]]]
      command = exit
      enabled = True
      name = exit
```