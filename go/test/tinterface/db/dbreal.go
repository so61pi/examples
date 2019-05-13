package db

import "strconv"

type DBReal struct{}

// Make sure that realDB implements DBInterface.
var _ DBInterface = &DBReal{}

func NewDBReal() *DBReal {
	return &DBReal{}
}

func (db *DBReal) GetUsername(id uint64) (string, error) {
	return strconv.FormatUint(id, 10), nil
}
