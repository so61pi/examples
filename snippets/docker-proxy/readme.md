## For client
- https://docs.docker.com/network/proxy/

## For daemon
- https://docs.docker.com/config/daemon/systemd/#httphttps-proxy
- To use sock proxy, we could set `ALL_PROXY`

```
[Service]
Environment="ALL_PROXY=socks5://<proxy-address>:<proxy-port>/"
```
