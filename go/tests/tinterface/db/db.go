package db

type DBInterface interface {
	GetUsername(id uint64) (string, error)
}
