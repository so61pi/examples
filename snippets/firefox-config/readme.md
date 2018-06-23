## about:config

### Documents

- https://dxr.mozilla.org/mozilla-central/source/modules/libpref/init/all.js
- https://superuser.com/questions/697018/how-to-disable-popups-in-firefox-without-add-ons
- http://kb.mozillazine.org/About:config_entries

### Popups prevention

| Field                                         | Value |
| --------------------------------------------- | ----- |
| dom.popup_maximum                             |     2 |
| dom.popup_allowed_events                      |  none |
| dom.disable_window_open_feature.location      |  true |
| dom.disable_window_open_feature.close         |  true |
| dom.disable_window_open_feature.resizable     |  true |
| dom.disable_window_open_feature.minimizable   |  true |
| dom.disable_window_move_resize                |  true |
| browser.link.open_newwindow                   |     3 |
| browser.link.open_newwindow.restriction       |     0 |
| browser.link.open_newwindow.override.external |    -1 |

### Keep Firefox open after closing the last tab

| Field                                         | Value |
| --------------------------------------------- | ----- |
| browser.tabs.closeWindowWithLastTab           | false`|

## Add-ons

- HTTPS Everywhere
- Mute Tab (WebExtension)
- Tree Style Tab
- Facebook Container
- To Google Translate

## desktop file `~/.local/share/applications/firefox-nightly.desktop`

```
[Desktop Entry]
Version=1.0
Name=Nightly
Comment=Browse the World Wide Web
Icon=/opt/firefox/browser/chrome/icons/default/default128.png
Exec=/opt/firefox/firefox %u
Terminal=false
Type=Application
Categories=Network;WebBrowser;
```

## Dark `about:newtab` & `about:blank`

- Menu `Help` > `Troubleshooting Information`.
- Find `Profile Directory`, click `Open Directory`.
- Create `chrome/userContent.css` with following content:

```css
@-moz-document url(about:blank) {
  html {
    background-color: #393F4C !important;
  }
}

@-moz-document url-prefix(about:blank) {
  * {
    background-color: #393F4C;
  }
}

@-moz-document url(about:newtab) {
  html {
    background-color: #393F4C !important;
  }
}

@-moz-document url-prefix(about:newtab) {
  * {
    background-color: #393F4C;
  }
}
```
