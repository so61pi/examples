IMAGETAG	:= so61pi/firefox-builder

GUIOPT		:= --net=host -e DISPLAY --volume=$(XAUTHORITY):/home/builder/.Xauthority
SECOPT		:= --cap-add=SYS_PTRACE --security-opt seccomp=unconfined
SRCMOUNT	:= -v $(shell pwd)/../mozilla-central:/mozilla-central:rw
ORGSRCMOUNT	:= -v $(shell pwd)/../orig-mozilla-central:/mozilla-central:rw


.PHONY: all
all: docker-compile-firefox


.PHONY: docker-build
docker-build:
	docker build --build-arg groupid=$(shell id -g) --build-arg userid=$(shell id -u) -t $(IMAGETAG) .


.PHONY: docker-compile-firefox
docker-compile-firefox:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(SRCMOUNT) $(IMAGETAG)


.PHONY: orig-docker-compile-firefox
orig-docker-compile-firefox:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(ORGSRCMOUNT) $(IMAGETAG)


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(GUIOPT) $(SRCMOUNT) -v $(shell pwd)/arcrc:/home/builder/.arcrc:ro $(IMAGETAG) fish


.PHONY: orig-docker-shell
orig-docker-shell:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(GUIOPT) $(ORGSRCMOUNT) $(IMAGETAG) fish


.PHONY: run
run:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(GUIOPT) $(SRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


.PHONY: orig-run
orig-run:
	docker run -it --rm --init --env-file Dockerenv.list $(SECOPT) $(GUIOPT) $(ORGSRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)
