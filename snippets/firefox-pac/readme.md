**PAC file should be named `proxy.pac`**

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
