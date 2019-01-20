package memory

import (
	"examples/go/library/server/db"
	modelsv1 "examples/go/library/server/pkg/models/v1"
)

type dbClient struct {
	books map[int64]modelsv1.Book
}

func NewClient() (db.Database, error) {
	return &dbClient{}, nil
}

func (dbc *dbClient) ListBooks() ([]modelsv1.Book, error) {
	books := make([]modelsv1.Book, len(dbc.books))
	for _, v := range dbc.books {
		books = append(books, v)
	}
	return books, nil
}

func (dbc *dbClient) AddBooks(books []modelsv1.Book) error {
	for _, book := range books {
		dbc.books[book.Id] = book
	}
	return nil
}

func (dbc *dbClient) GetBookById(id int64) (*modelsv1.Book, error) {
	if book, ok := dbc.books[id]; ok {
		return &book, nil
	}

	return nil, db.ErrResourceNotFound
}
