package main

import (
	"fmt"
	"log"
	"net/http"
	_ "net/http/pprof"
)

func fib(n uint) uint64 {
	if n < 2 {
		return 1
	}
	return fib(n-1) + fib(n-2)
}

func mem(n uint) {
	x := make([]string, 0)
	for ; n > 0; n-- {
		x = append(x, "test")
	}
}

func main() {
	http.HandleFunc("/cpu", func(w http.ResponseWriter, r *http.Request) {
		fib(40)
		fmt.Fprintln(w, "/cpu done")
	})
	http.HandleFunc("/mem", func(w http.ResponseWriter, r *http.Request) {
		mem(1000000)
		fmt.Fprintln(w, "/mem done")
	})

	log.Println("running")
	log.Println(http.ListenAndServe("localhost:6060", nil))
}
