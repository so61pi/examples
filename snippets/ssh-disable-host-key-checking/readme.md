## Oneshot

```shell
 ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no example.com
```

## Permanent

- Put it to `~/.ssh/config` and narrow the file permission by `chmod 600 ~/.ssh/config`.

```txt
Host example.*
    StrictHostKeyChecking no
    UserKnownHostsFile /dev/null
```
