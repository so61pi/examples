IMAGETAG	:= firefox-builder


.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build --rm=true --build-arg groupid=$(shell id -g) --build-arg userid=$(shell id -u) -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -it --rm --init --env-file Dockerenv.list -v $(shell pwd)/../mozilla-central:/mozilla-central:rw $(IMAGETAG)


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init --env-file Dockerenv.list -v $(shell pwd)/../mozilla-central:/mozilla-central:rw $(IMAGETAG) fish


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)
