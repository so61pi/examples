- Stop docker service

```shell
sudo systemctl stop docker
```


- Add the following configuration to `/etc/docker/daemon.json`

```json
{
    "tls": false
}
```


- Start docker service

```shell
sudo systemctl start docker
```
