package db

import (
	"examples/go/urlshortener/model"
	"examples/go/urlshortener/model/mongodb"
	"examples/go/urlshortener/model/redis"
)

type DB interface {
	AddUrl(url string) (*model.UrlInfo, error)
	GetUrl(url string) (*model.UrlInfo, error)
	HitUrl(url string) error
	DelUrl(url string) error

	UrlLst() ([]model.UrlInfo, error)

	Close()
}


func NewClient(data interface{}) (DB, error) {
	switch data.(type) {
	case mongodb.Config:
		return mongodb.NewClient(data.(mongodb.Config))

	case redis.Config:
		return redis.NewClient(data.(redis.Config))

	default:
		panic("invalid config")
	}
}
