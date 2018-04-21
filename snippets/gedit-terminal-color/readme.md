## For ubuntu 14

```shell
gsettings set org.gnome.gedit.plugins.terminal use-theme-colors false
gsettings set org.gnome.gedit.plugins.terminal background-color "#300A24"
gsettings set org.gnome.gedit.plugins.terminal foreground-color "#FFFFFF"
```

## For ubuntu 16

- Open file `/usr/lib/x86_64-linux-gnu/gedit/plugins/terminal.py` (you can find it with `find /usr/lib -name terminal.py`)
- Find `def reconfigure_vte(self):`
- Add

```txt
fg = Gdk.RGBA(1, 1, 1, 1)
bg = Gdk.RGBA(0.1875, 0.0390625, 0.140625, 1)
```

- Save
