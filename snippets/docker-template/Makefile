IMAGETAG	:= image-test

GUIOPT		:= --net=host -e DISPLAY --volume=$(XAUTHORITY):/home/builder/.Xauthority


.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build --build-arg groupid=$(shell id -g) --build-arg userid=$(shell id -u) -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -it --rm --init -e "TERM=xterm-256color" $(GUIOPT) -v $(shell pwd):/code:ro $(IMAGETAG) ls -alh


.PHONY: docker-run-detach
docker-run-detach:
	docker run --detach --rm --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro $(IMAGETAG) sleep 30


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro $(IMAGETAG) bash


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)

