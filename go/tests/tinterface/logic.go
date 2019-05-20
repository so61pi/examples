package tinterface

import "examples/go/tests/tinterface/db"

type logic struct {
	db db.DBInterface
}

func NewLogic(db db.DBInterface) *logic {
	return &logic{
		db: db,
	}
}

func (l *logic) IsRoot(id uint64) (bool, error) {
	if name, err := l.db.GetUsername(id); err != nil {
		return false, err
	} else {
		return name == "root", nil
	}
}
