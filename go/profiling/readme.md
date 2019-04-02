## `main.go`

#### Run program and generate profile files.

```shell
go build main.go && ./main
```

#### View profile files.

```shell
go tool pprof -http localhost:8888 main example-cpu.prof
go tool pprof -http localhost:8888 main example-mem.prof
```

#### References
  - https://golang.org/pkg/runtime/pprof/

## `main-web.go`

#### Run web server.

```shell
go run main-web.go
```

#### View memory profile.
##### Trigger memory usage

```shell
curl http://localhost:6060/mem
```

##### View profile

```shell
go tool pprof -http localhost:8888 http://localhost:6060/debug/pprof/heap
```

#### View CPU profile
##### Start collecting profile before triggering CPU usage

```shell
go tool pprof -http localhost:8888 'http://localhost:6060/debug/pprof/profile?seconds=60'
```

##### Trigger CPU usage

```shell
curl http://localhost:6060/cpu
```

#### References
  - http://localhost:6060/debug/pprof/
  - https://golang.org/pkg/net/http/pprof/
