**oneshot**

```shell
 ssh -o "ProxyCommand corkscrew <proxy-address> 8080 <ssh-server> 22" <ssh-server>
```


**permanent**

- Put this to your `/etc/ssh/ssh_config`.
- In case you only want this config for your user only, put it to `~/.ssh/config` and narrow the file permission by `chmod 400 ~/.ssh/config`.

```
Host example.*
    ProxyCommand corkscrew <proxy-address> 8080 %h %p
```
