## Build docker image

```shell
docker build --rm=true -t ubuntu/fluxbox .
```

## Run docker image

```shell
docker run -i -t -d -p 5900 -p 22 ubuntu/fluxbox /root/envup
```

## NOTE

- The last command that is run in container must not exit or daemonize, because container stops when last command stops.

## Links

- [docker run](https://docs.docker.com/engine/reference/run/)
- [Dockerfile](https://docs.docker.com/engine/reference/builder/)

## Other useful commands

- Attach to a running container

```shell
docker attach <container-hash-id>
# Use ^P^Q (Ctrl + P, Ctrl + Q) to escape
```

- Start a stopped container

```shell
docker start <container-id>
```

- Stop a container

```shell
docker stop <container-id>
```

- Show containers and images

```shell
docker ps -a
docker images -a
```

- Delete container and image

```shell
docker rm <container-id>
docker rmi <image-id>
```

- List all untagged images that are not used by other images

```shell
docker images -a -f dangling=true
```
