.PHONY: all
all:
	glide install
	go build main.go

.PHONY: docker
docker:
	docker build --tag examples-library-go -f scripts/Dockerfile .
