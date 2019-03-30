IMAGETAG	:= downloader-work-test

USERNAME	:= worker

OPTIONS		:= -v $(shell pwd)/code:/code:rw
OPTIONS		+= -e "TERM=xterm-256color" -p 4200:4200

BUILDARGS	:= --build-arg groupid=$(shell id -g)
BUILDARGS	+= --build-arg userid=$(shell id -u)
BUILDARGS	+= --build-arg username=$(USERNAME)
BUILDARGS	+= --build-arg gitemail="$(shell git config user.email)"
BUILDARGS	+= --build-arg gituser="$(shell git config user.name)"


.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build $(BUILDARGS) -t $(IMAGETAG) scripts/docker


.PHONY: docker-run
docker-run:
	docker run -it --rm --init $(OPTIONS) $(IMAGETAG)


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
