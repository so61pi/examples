package main

import (
	apiv1 "examples/go/library/server/api/v1"
	"examples/go/library/server/db/memory"
	"examples/go/library/server/router"
)

func main() {
	db, err := memory.NewClient()
	if err != nil {
		panic(err)
	}

	api, err := apiv1.NewApi(db)
	if err != nil {
		panic(err)
	}

	router.NewRouter(api).Run(":58088")
}
