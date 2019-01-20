package db

import (
	"errors"
	modelsv1 "examples/go/library/server/pkg/models/v1"
)

type Database interface {
	ListBooks() ([]modelsv1.Book, error)
	AddBooks([]modelsv1.Book) error
	GetBookById(int64) (*modelsv1.Book, error)
}

var (
	ErrResourceNotFound = errors.New("requested resource not found")
)
