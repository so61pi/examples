# KDE Plasma Setup (openSUSE)

## Global shortcuts

### Maximize windows

```shell
# Enable Meta+Up
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Maximize","KWin","Fenster maximieren" array:int32:285212691 uint32:4

# Disable Meta+Up
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Maximize","KWin","Fenster maximieren" array:int32:0 uint32:4
```

### Minimize windows

```shell
# Enable Meta+Down
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Minimize","KWin","Fenster minimieren" array:int32:285212693 uint32:4

# Disable Meta+Down
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Minimize","KWin","Fenster minimieren" array:int32:0 uint32:4
```