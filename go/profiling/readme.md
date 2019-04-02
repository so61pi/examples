```shell
go build main.go && ./main

go tool pprof -http localhost:8888 main example-cpu.prof
go tool pprof -http localhost:8888 main example-mem.prof
```
