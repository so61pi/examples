package db

import (
	"examples/go/urlshortener/model"
)

type DB interface {
	AddUrl(url string) (*model.UrlInfo, error)
	GetUrl(url string) (*model.UrlInfo, error)
	HitUrl(url string) error
	DelUrl(url string) error

	UrlLst() ([]model.UrlInfo, error)

	Close()
}
