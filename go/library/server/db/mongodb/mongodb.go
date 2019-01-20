package mongodb

import (
	"examples/go/library/server/db"
	modelsv1 "examples/go/library/server/pkg/models/v1"
	"github.com/globalsign/mgo"
)

type dbClient struct{}

func NewClient() (db.Database, error) {
	panic("TODO: Unimplemented")
}

func (db *dbClient) ListBooks() ([]modelsv1.Book, error) {
	panic("TODO: Unimplemented")
}

func (db *dbClient) AddBooks(books []modelsv1.Book) error {
	panic("TODO: Unimplemented")
}

func (db *dbClient) GetBookById(id int64) (*modelsv1.Book, error) {
	panic("TODO: Unimplemented")
}
