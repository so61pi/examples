**oneshot**

```shell
 ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no example.com
```


**permanent**

- Put it to `~/.ssh/config` and narrow the file permission by `chmod 600 ~/.ssh/config`.

```
Host example.*
    StrictHostKeyChecking no
    UserKnownHostsFile /dev/null
```
