**Enable maximize window shortcut**

- Add following XML snippet into `$HOME/.config/openbox/lubuntu-rc.xml`, under `/openbox_config/keyboard`.

```xml
    <keybind key="C-W-Up">        # FullScreen
      <action name="Maximize"/>
    </keybind>
```
