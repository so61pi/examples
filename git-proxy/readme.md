## Set SOCKS proxy through env

- SOCKS proxy

```shell
# Create SOCKS proxy
ssh -N -f -D localhost:3000 admin@server.com

# Use SOCKS proxy
export ALL_PROXY=socks5h://127.0.0.1:3000
```

- `http_proxy` and `https_proxy` can also be set through env

```shell
# curl doesn't understand upper case versions of http_proxy and https_proxy
export http_proxy=http://<proxy-address>:<proxy-port>
export https_proxy=https://<proxy-address>:<proxy-port>
```

## Set proxy for git protocol (`git://`)

- Create port forwarding

```shell
# git protocol uses port 9418
# All connections to 127.0.0.1:9418 are forwarded to git.denx.de.com:9418
ssh -N -f -L 9418:git.denx.de.com:9418 admin@server.com
```

- Add following lines to `/etc/hosts`

```txt
# git://git.denx.de/u-boot.git
127.0.0.1    git.denx.de
```

## Set `http.proxy` and `https.proxy` permanently

```shell
git config --global http.proxy http://<proxy-address>:<proxy-port>
git config --global https.proxy http://<proxy-address>:<proxy-port>

git config --global --unset http.proxy
git config --global --unset https.proxy
```

## Set SOCKS proxy permanently

- Put this to your `/etc/ssh/ssh_config`.
- In case you only want this config for your user only, put it to `~/.ssh/config` and narrow the file permission by `chmod 400 ~/.ssh/config`.

```txt
Host bitbucket.org
  User git
  ProxyCommand nc -X 5 -x <proxy-address>:<proxy-port> %h %p
```

## Enable network debugging

```shell
export GIT_CURL_VERBOSE=1
```
