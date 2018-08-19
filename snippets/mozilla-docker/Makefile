IMAGETAG	:= so61pi/firefox-builder

USERNAME	:= builder

OPTIONS		:= --net=host -e DISPLAY --volume=$(XAUTHORITY):/home/$(USERNAME)/.Xauthority
OPTIONS		+= --cap-add=SYS_PTRACE --security-opt seccomp=unconfined
OPTIONS		+= --sysctl net.ipv6.conf.all.disable_ipv6=1 --hostname localhost
OPTIONS		+= -v $(shell pwd)/arcrc:/home/$(USERNAME)/.arcrc:ro
OPTIONS		+= -v $(shell pwd)/fish_history:/home/$(USERNAME)/.local/share/fish/fish_history:rw

SRCMOUNT	:= -v $(shell pwd)/../mozilla-central:/mozilla-central:rw
ORGSRCMOUNT	:= -v $(shell pwd)/../orig-mozilla-central:/mozilla-central:rw

BUILDARGS	:= --build-arg groupid=$(shell id -g)
BUILDARGS	+= --build-arg userid=$(shell id -u)
BUILDARGS	+= --build-arg username=$(USERNAME)


.PHONY: all
all: docker-compile-firefox


.PHONY: auxfiles
auxfiles:
	touch fish_history


.PHONY: docker-build
docker-build:
	docker build $(BUILDARGS) -t $(IMAGETAG) .


.PHONY: docker-compile-firefox
docker-compile-firefox:
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(SRCMOUNT) $(IMAGETAG)


.PHONY: orig-docker-compile-firefox
orig-docker-compile-firefox:
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(ORGSRCMOUNT) $(IMAGETAG)


.PHONY: docker-shell
docker-shell: auxfiles
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(SRCMOUNT) $(IMAGETAG) fish


.PHONY: orig-docker-shell
orig-docker-shell: auxfiles
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(ORGSRCMOUNT) $(IMAGETAG) fish


.PHONY: run
run:
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(SRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


.PHONY: orig-run
orig-run:
	docker run -it --rm --init --env-file Dockerenv.list $(OPTIONS) $(ORGSRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)
