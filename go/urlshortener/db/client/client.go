package client

import (
	"examples/go/urlshortener/db"
	"examples/go/urlshortener/db/client/mongodb"
	"examples/go/urlshortener/db/client/redis"
)

func NewClient(data interface{}) (db.DB, error) {
	switch data.(type) {
	case mongodb.Config:
		return mongodb.NewClient(data.(mongodb.Config))

	case redis.Config:
		return redis.NewClient(data.(redis.Config))

	default:
		panic("invalid config")
	}
}
