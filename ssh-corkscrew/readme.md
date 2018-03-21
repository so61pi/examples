**oneshot**

```shell
 ssh -o "ProxyCommand corkscrew <proxy-address> <proxy-port> <ssh-server> 22" <ssh-server>
```


**permanent**

- Put it to `~/.ssh/config` and narrow the file permission by `chmod 600 ~/.ssh/config`.

```
Host example.*
    ProxyCommand corkscrew <proxy-address> <proxy-port> %h %p
```
