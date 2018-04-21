- Put this to your `/etc/ssh/ssh_config`.
- In case you only want this config for your user only, put it to `~/.ssh/config` and narrow the file permission by `chmod 400 ~/.ssh/config`.

```txt
Host bitbucket.org
  User git
  ProxyCommand nc -X 5 -x <proxy-address>:<proxy-port> %h %p
```
