package util

import (
	"crypto/rand"
	"encoding/base64"
	"strings"
)

func GenerateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	if _, err := rand.Read(b); err != nil {
		return nil, err
	}

	return b, nil
}

func GenerateRandomString(s int) (string, error) {
	b, err := GenerateRandomBytes(s)
	return strings.Replace(base64.URLEncoding.EncodeToString(b), "=", "", -1), err
}
