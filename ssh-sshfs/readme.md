**mount**

```shell
sshfs -o default_permissions,reconnect,no_readahead,cache=yes,kernel_cache,compression=no,Cipher=arcfour admin@<server>:<server-dir> <local-dir>
# sshfs -o allow_other,default_permissions,reconnect,no_readahead,cache=yes,kernel_cache,compression=no,Cipher=arcfour admin@<server>:<server-dir> <local-dir>
```


**unmount**

```shell
fusermount -u <local-dir>
```
