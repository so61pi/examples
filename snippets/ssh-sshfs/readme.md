## Mount

```shell
sshfs -o uid=0,gid=0,default_permissions,reconnect,no_readahead,cache=yes,kernel_cache,compression=no,Cipher=arcfour,ro admin@<server>:<server-dir> <local-dir>
```

## Unmount

```shell
fusermount -u <local-dir>
```
