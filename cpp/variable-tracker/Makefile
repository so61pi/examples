RM			?= rm
BUILDDIR	:= build
IMAGETAG	:= variable-tracker-builder

.PHONY: all
all:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake .. && cmake --build .

.PHONY: clean
clean:
	$(RM) -r $(BUILDDIR)

.PHONY: docker-build
docker-build:
	docker build --rm=true -t $(IMAGETAG) .

.PHONY: docker-run
docker-run:
	docker run --init -t -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code $(IMAGETAG) make all

.PHONY: docker-shell
docker-shell:
	docker run --init -i -t -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code $(IMAGETAG) bash
