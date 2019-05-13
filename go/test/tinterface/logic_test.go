package tinterface_test

import (
	"testing"

	"examples/go/test/tinterface"
	"examples/go/test/tinterface/db"
)

func TestWithMockInterface(t *testing.T) {
	// Construct new logic with a mock object.
	l := tinterface.NewLogic(newDBMock())

	if isRoot, _ := l.IsRoot(1); !isRoot {
		t.Failed()
	}
}

// dbMock is a mock interface.
type dbMock struct{}

var _ db.DBInterface = &dbMock{}

func newDBMock() *dbMock {
	return &dbMock{}
}

func (db *dbMock) GetUsername(uint64) (string, error) {
	return "root", nil
}
