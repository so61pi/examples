## Oneshot

```shell
ssh -o "ProxyCommand corkscrew <proxy-address> <proxy-port> <ssh-server> 22" <ssh-server>
```

## Permanent

- Put it to `~/.ssh/config` and narrow the file permission by `chmod 600 ~/.ssh/config`.

```txt
Host example.*
    ProxyCommand corkscrew <proxy-address> <proxy-port> %h %p
```
