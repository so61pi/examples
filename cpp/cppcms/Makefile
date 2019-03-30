IMAGETAG	:= image-cppcms


.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build --rm=true -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -it --rm --init -e "TERM=xterm-256color" -p 58080:8080 -v $(shell pwd):/code:ro -w /code $(IMAGETAG) ./run.sh


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init -e "TERM=xterm-256color" -p 58080:8080 -v $(shell pwd):/code:ro -w /code $(IMAGETAG) bash


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)

