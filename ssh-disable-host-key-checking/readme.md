**oneshot**

```shell
 ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no example.com
```


**permanent**

- Put this to your `/etc/ssh/ssh_config`.
- In case you only want this config for your user only, put it to `~/.ssh/config` and narrow the file permission by `chmod 400 ~/.ssh/config`.

```
Host example.*
    StrictHostKeyChecking no
    UserKnownHostsFile /dev/null
```
