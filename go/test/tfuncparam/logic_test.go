package tfuncparam_test

import (
	"testing"

	"examples/go/test/tfuncparam"
)

func TestWithFuncParam(t *testing.T) {
	if isRoot, _ := tfuncparam.IsRoot(1, mockGetUsername); !isRoot {
		t.Failed()
	}
}

func mockGetUsername(id uint64) (string, error) {
	return "root", nil
}
