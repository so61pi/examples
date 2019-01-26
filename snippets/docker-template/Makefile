IMAGETAG	:= image-test

USERNAME	:= worker

OPTIONS		:= --net=host -e DISPLAY --volume=$(XAUTHORITY):/home/$(USERNAME)/.Xauthority
OPTIONS		+= --sysctl net.ipv6.conf.all.disable_ipv6=1 --hostname localhost
OPTIONS		+= -v $(shell pwd):/code:ro
OPTIONS		+= -e "TERM=xterm-256color"

BUILDARGS	:= --build-arg groupid=$(shell id -g)
BUILDARGS	+= --build-arg userid=$(shell id -u)
BUILDARGS	+= --build-arg username=$(USERNAME)
#BUILDARGS	+= --build-arg gitemail=$(shell git config user.email)
#BUILDARGS	+= --build-arg gituser=$(shell git config user.name)

.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build $(BUILDARGS) -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -it --rm --init $(OPTIONS) $(IMAGETAG) ls -alh


.PHONY: docker-run-detach
docker-run-detach:
	docker run --detach --rm --init $(OPTIONS) $(IMAGETAG) sleep 30


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init $(OPTIONS) $(IMAGETAG) bash


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)

