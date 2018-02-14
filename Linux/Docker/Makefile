IMAGETAG	:= image-test


.PHONY: docker-build
docker-build:
	docker build --rm=true -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -t --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro -w /code $(IMAGETAG) ls -alh


.PHONY: docker-shell
docker-shell:
	docker run -i -t --init -e "TERM=xterm-256color" -v $(shell pwd):/code:ro -w /code $(IMAGETAG) bash


# NOTE: The docker-clean is used to clean ALL the used containers and images in the system.
.PHONY: docker-clean
docker-clean:
	docker image prune -f
	docker container prune -f


.PHONY: docker-rm
docker-rm:
	docker image rm -f $(IMAGETAG)

