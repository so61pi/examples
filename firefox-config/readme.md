## about:config

### Documents

- https://dxr.mozilla.org/mozilla-central/source/modules/libpref/init/all.js
- https://superuser.com/questions/697018/how-to-disable-popups-in-firefox-without-add-ons
- http://kb.mozillazine.org/About:config_entries

### Popups prevention

| Field                                         | Value |
| --------------------------------------------- | ----- |
| dom.popup_maximum                             |     2 |
| dom.popup_allowed_events                      |       |
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

### Open new tab after current tab

| Field                                         | Value |
| --------------------------------------------- | ----- |
| browser.tabs.insertAfterCurrent               |  true |

### Show tabs in full-screen mode

| Field                                         | Value |
| --------------------------------------------- | ----- |
| browser.fullscreen.autohide                   | false |

### Hide title bar

| Field                                         | Value |
| --------------------------------------------- | ----- |
| browser.tabs.drawInTitleBar                   |  true |

## Add-ons

- Dark Reader
- Download all Images
- Facebook Container
- HTTPS Everywhere
- KeePassXC-Browser
- LeechBlock
- Mate Translate
- Mute sites by default
- Privacy Badger
- To Google Translate
- uBlock Origin

## desktop file `~/.local/share/applications/firefox-nightly.desktop`

```
[Desktop Entry]
Categories=Network;WebBrowser;
Encoding=UTF-8
Name=Firefox Nightly
GenericName=Web Browser
Comment=Web Browser
Icon=/opt/firefox/browser/chrome/icons/default/default128.png
Exec=/opt/firefox/firefox %u
Terminal=false
StartupNotify=true
MimeType=text/html;text/xml;application/xhtml+xml;application/vnd.mozilla.xul+xml;text/mml;application/x-xpinstall;x-scheme-handler/http;x-scheme-handler/https;x-scheme-handler/ftp;
Type=Application
```

## PAC

PAC file should be named `proxy.pac`

```js
function FindProxyForURL(url, host) {
    url = url.toLowerCase();
    host = host.toLowerCase();

    let patterns =
    [
        "*example.com",
        "*example.net"
    ];

    for (let p of patterns) {
        if (shExpMatch(host, p)) {
            return "SOCKS5 <proxy-server>:<proxy-port>; DIRECT";
        }
    }

    return "DIRECT";
}
```
