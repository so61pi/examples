IMAGETAG	:= image-zsh-test


.PHONY: all
all: docker-run


.PHONY: docker-build
docker-build:
	docker build -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -it --rm --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro -w /code $(IMAGETAG) ls -alh


.PHONY: docker-run-detach
docker-run-detach:
	docker run --detach --rm --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro -w /code $(IMAGETAG) sleep 30


.PHONY: docker-shell
docker-shell:
	docker run -it --rm --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro -w /code $(IMAGETAG) zsh


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)

