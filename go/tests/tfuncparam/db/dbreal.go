package db

import "strconv"

type DBReal struct{}

func NewDBReal() *DBReal {
	return &DBReal{}
}

func (db *DBReal) GetUsername(id uint64) (string, error) {
	return strconv.FormatUint(id, 10), nil
}
