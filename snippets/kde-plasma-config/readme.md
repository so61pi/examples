# KDE Plasma Setup (openSUSE)

## Note

- Use `dbus-monitor` to monitor dbus message

## Global shortcuts

### Maximize window

```shell
# Enable Meta+Up
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Maximize","KWin","Fenster maximieren" array:int32:285212691 uint32:4

# Disable Meta+Up
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Maximize","KWin","Fenster maximieren" array:int32:0 uint32:4
```

### Minimize window

```shell
# Enable Meta+Down
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Minimize","KWin","Fenster minimieren" array:int32:285212693 uint32:4

# Disable Meta+Down
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Minimize","KWin","Fenster minimieren" array:int32:0 uint32:4
```

### Tile window to right

```shell
# Enable Meta+Right
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Quick Tile Right","KWin","Fenster am rechten Bildschirmrand anordnen" array:int32:285212692 uint32:4

# Disable Meta+Right
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Quick Tile Right","KWin","Fenster am rechten Bildschirmrand anordnen" array:int32:0 uint32:4
```

### Tile window to left

```shell
# Enable Meta+Left
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Quick Tile Left","KWin","Fenster am linken Bildschirmrand anordnen" array:int32:285212690 uint32:4

# Disable Meta+Left
dbus-send --session --type=method_call --print-reply --dest=org.kde.kglobalaccel /kglobalaccel org.kde.KGlobalAccel.setShortcut array:string:"kwin","Window Quick Tile Left","KWin","Fenster am linken Bildschirmrand anordnen" array:int32:0 uint32:4
```
