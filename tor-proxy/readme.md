## Download + Setup

- https://www.torproject.org/download/download-easy.html.en

## Run `tor`

- Linux

```shell
cd Browser/TorBrowser/Tor
LD_LIBRARY_PATH=`pwd` ./tor
```

- Windows
  - Go to `Browser\TorBrowser\Tor`, run `tor.exe`.

## Config FireFox

- Configure proxy manually.
  - Open Firefox.
  - Open `about:preferences#advanced` page, click `Settings...`.
  - Choose `Manual proxy configuration`.
  - Only fill `SOCKS Host`, leave other proxy fields empty.
    - Address: `127.0.0.1`
    - Port: `9050`
- Create `proxy.pac` with the following content:

```js
function FindProxyForURL(url, host) {
    if (shExpMatch(host, "*facebook.com")) {
        return "SOCKS 127.0.0.1:9050";
    }
    if (shExpMatch(host, "*fbcdn.net")) {
        return "SOCKS 127.0.0.1:9050";
    }
    return "DIRECT";
}
```

- Using `proxy.pac`.
  - Open Firefox.
  - Open `about:preferences#advanced` page, click `Settings...`.
  - Choose `Automatic proxy configuration URL`.
  - Fill `file:///proxy.pac`.

## Option for `tor` and `torrc` format

- https://www.torproject.org/docs/tor-manual.html.en
