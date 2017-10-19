RM			?= rm
IMAGETAG	:= image-test


.PHONY: docker-build
docker-build:
	docker build --rm=true -t $(IMAGETAG) .


.PHONY: docker-run
docker-run:
	docker run -t --init -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code $(IMAGETAG) ls -alh
