PROJECT		:= testsite
APP			:= testapp
IMAGETAG	:= django-test-image

CONPORT		:= 8000
HOSTPORT	:= 58000


.PHONY: docker-build
docker-build:
	docker build --rm=true -t $(IMAGETAG) .


.PHONY: docker-project
docker-project:
	docker run -t --init -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code $(IMAGETAG) django-admin startproject $(PROJECT) && cd $(PROJECT) && python manage.py migrate


.PHONY: docker-app
docker-app:
	docker run -t --init -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code/$(PROJECT) $(IMAGETAG) python manage.py startapp $(APP)


.PHONY: docker-run
docker-run:
	docker run -ti --init -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code/$(PROJECT) -p $(HOSTPORT):$(CONPORT) $(IMAGETAG) python manage.py runserver 0.0.0.0:$(CONPORT)


.PHONY: docker-terminal
docker-terminal:
	docker run -ti --init -e "TERM=xterm" -u $(shell id -u $$USER) -v $(shell pwd):/code -w /code/$(PROJECT) $(IMAGETAG) /bin/bash
