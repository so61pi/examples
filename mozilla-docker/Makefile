IMAGETAG	:= so61pi/firefox-builder

USERNAME	:= builder

RUNOPTIONS	:= -it --rm --init --env-file Dockerenv.list
RUNOPTIONS	+= --net=host -e DISPLAY --volume=$(XAUTHORITY):/home/$(USERNAME)/.Xauthority
RUNOPTIONS	+= --cap-add=SYS_PTRACE --security-opt seccomp=unconfined
RUNOPTIONS	+= --sysctl net.ipv6.conf.all.disable_ipv6=1 --hostname localhost
RUNOPTIONS	+= -v $(shell pwd)/arcrc:/home/$(USERNAME)/.arcrc:ro
RUNOPTIONS	+= -v $(shell pwd)/fish_history:/home/$(USERNAME)/.local/share/fish/fish_history:rw

SRCMOUNT	:= -v $(shell pwd)/../mozilla-central:/mozilla-central:rw
JSSRCMOUNT	:= -v $(shell pwd)/../js-mozilla-central:/mozilla-central:rw
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
	docker run $(RUNOPTIONS) $(SRCMOUNT) $(IMAGETAG)


.PHONY: orig-docker-compile-firefox
orig-docker-compile-firefox:
	docker run $(RUNOPTIONS) $(ORGSRCMOUNT) $(IMAGETAG)


.PHONY: docker-shell
docker-shell: auxfiles
	docker run $(RUNOPTIONS) $(SRCMOUNT) $(IMAGETAG) fish


.PHONY: js-docker-shell
js-docker-shell: auxfiles
	docker run -w=/mozilla-central/js/src $(RUNOPTIONS) $(JSSRCMOUNT) $(IMAGETAG) fish


.PHONY: orig-docker-shell
orig-docker-shell: auxfiles
	docker run $(RUNOPTIONS) $(ORGSRCMOUNT) $(IMAGETAG) fish


.PHONY: run
run:
	docker run $(RUNOPTIONS) $(SRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


.PHONY: orig-run
orig-run:
	docker run $(RUNOPTIONS) $(ORGSRCMOUNT) $(IMAGETAG) fish -c '/mozilla-central/mach run'


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)
