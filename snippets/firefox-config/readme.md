## Block all popups

- Go to `about:config`.
- Change `dom.popup_allowed_events` to empty.

## Always open popup in new tab

- Go to `about:config`.
- Change `browser.link.open_newwindow.restriction` to 0.
- Make sure `browser.link.open_newwindow` is set to 3 (default value).

## Keep Firefox open after closing the last tab

- Go to `about:config`
- Change `browser.tabs.closeWindowWithLastTab` to `false`

## Add-ons

- HTTPS Everywhere
- Mute Tab (WebExtension)

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
