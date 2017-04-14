1. Build docker image

        docker build --rm=true -t ubuntu/fluxbox .


2. Run docker image

        docker run -i -t -d -p 5900 -p 22 ubuntu/fluxbox /root/envup

3. NOTE
    - The last command that is run in container must not exit or daemonize, because container stops when last command stops.


4. Links
    - [docker run](https://docs.docker.com/engine/reference/run/)
    - [Dockerfile](https://docs.docker.com/engine/reference/builder/)


5. Other useful commands
    - Attach to a running container

            docker attach <container-hash-id>
            # Use ^P^Q (Ctrl + P, Ctrl + Q) to escape

    - Start a stopped container

            docker start <container-id>

    - Stop a container

            docker stop <container-id>

    - Show containers and images

            docker ps -a
            docker images -a

    - Delete container and image

            docker rm <container-id>
            docker rmi <image-id>

    - List all untagged images that are not used by other images

            docker images -a -f dangling=true
