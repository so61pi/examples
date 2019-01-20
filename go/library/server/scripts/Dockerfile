FROM golang:1.11

RUN mkdir -p /go/src/examples/go/library/server
WORKDIR /go/src/examples/go/library/server

ADD . .
RUN go build main.go

USER nobody
EXPOSE 58080
CMD ["./main"]
