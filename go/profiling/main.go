package main

import (
	"log"
	"os"
	"runtime"
	"runtime/pprof"
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
	// CPU Profile
	if cpuProfile, err := os.Create("example-cpu.prof"); err != nil {
		log.Fatal(err)
	} else {
		defer cpuProfile.Close()
		if err := pprof.StartCPUProfile(cpuProfile); err != nil {
			log.Fatal(err)
		}
		defer pprof.StopCPUProfile()
	}

	fib(40)
	mem(1000000)

	// Memory Profile
	runtime.GC() // get up-to-date statistics
	if memProfile, err := os.Create("example-mem.prof"); err != nil {
		log.Fatal(err)
	} else {
		defer memProfile.Close()
		if err := pprof.WriteHeapProfile(memProfile); err != nil {
			log.Fatal(err)
		}
	}
}
