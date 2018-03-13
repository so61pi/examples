```shell
sudo sshfs -o allow_other,default_permissions,reconnect,no_readahead,cache=yes,kernel_cache,compression=no,Cipher=arcfour admin@<server>:<server-dir> <local-dir>
```
