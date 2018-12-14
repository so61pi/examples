package client

import (
	"examples/go/urlshortener/db"
	"examples/go/urlshortener/db/client/mongodb"
	"examples/go/urlshortener/db/client/redis"
	"fmt"
)

func NewClient(data interface{}) (db.DB, error) {
	switch cfg := data.(type) {
	case mongodb.Config:
		return mongodb.NewClient(cfg)

	case *mongodb.Config:
		return mongodb.NewClient(*cfg)

	case redis.Config:
		return redis.NewClient(cfg)

	case *redis.Config:
		return redis.NewClient(*cfg)

	default:
		fmt.Printf("I don't know about type %T!\n", cfg)
		panic("invalid config")
	}
}
