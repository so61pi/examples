package main

import (
	"fmt"

	"github.com/pkg/errors"
)

func DBConnect(addr string) (interface{}, error) {
	switch addr {
	case "valid":
		return struct{}{}, nil

	case "invalid":
		return nil, errors.New("cannot connect to db")

	default:
		return nil, errors.New("another error")
	}
}

func DBGet(conn interface{}, key string) (string, error) {
	switch key {
	case "admin":
		return "admin", nil

	default:
		return "", errors.New("key not found")
	}
}

type GetPasswordErrorKind int

const (
	GetPasswordErrorCannotConnectDB = iota
	GetPasswordErrorUserNotFound
)

type GetPasswordError struct {
	kind  GetPasswordErrorKind
	cause error
}

func NewGetPasswordError(kind GetPasswordErrorKind, cause error) GetPasswordError {
	return GetPasswordError{
		kind:  kind,
		cause: cause,
	}
}

func (err GetPasswordError) Kind() GetPasswordErrorKind {
	return err.kind
}

func (err GetPasswordError) Cause() error {
	return err.cause
}

func (err GetPasswordError) Error() string {
	switch err.kind {
	case GetPasswordErrorCannotConnectDB:
		return "GetPasswordError: CannotConnectDB"

	case GetPasswordErrorUserNotFound:
		return "GetPasswordError: UserNotFound"

	default:
		panic("unreachable")
	}
}

func GetPassword(dbaddr string, user string) (string, error) {
	conn, err := DBConnect(dbaddr)
	if err != nil {
		return "", NewGetPasswordError(GetPasswordErrorCannotConnectDB, err)
	}

	pass, err := DBGet(conn, user)
	if err != nil {
		return "", NewGetPasswordError(GetPasswordErrorUserNotFound, err)
	}

	return pass, nil
}

type AuthErrorKind int

const (
	AuthErrorCannotConnectDB = iota
	AuthErrorUserNotFound
	AuthErrorInvalidPassword
)

type AuthError struct {
	kind  AuthErrorKind
	cause error
}

func NewAuthError(kind AuthErrorKind, cause error) AuthError {
	return AuthError{
		kind:  kind,
		cause: cause,
	}
}

func (err AuthError) Kind() AuthErrorKind {
	return err.kind
}

func (err AuthError) Cause() error {
	return err.cause
}

func (err AuthError) Error() string {
	switch err.kind {
	case AuthErrorCannotConnectDB:
		return "AuthError: CannotConnectDB"

	case AuthErrorUserNotFound:
		return "AuthError: UserNotFound"

	case AuthErrorInvalidPassword:
		return "AuthError: InvalidPassword"

	default:
		panic("unreachable")
	}
}

func Auth(dbaddr, user, pass string) error {
	dbpass, err := GetPassword(dbaddr, user)
	switch err.(GetPasswordError).Kind() {
	case GetPasswordErrorCannotConnectDB:
		return NewAuthError(AuthErrorCannotConnectDB, err)

	case GetPasswordErrorUserNotFound:
		return NewAuthError(AuthErrorUserNotFound, err)
	}

	if pass == dbpass {
		return NewAuthError(
			AuthErrorInvalidPassword,
			errors.New("invalid password"),
		)
	}

	return nil
}

func main() {
	fmt.Println("--------------------")
	if err := Auth("invalid", "root", "root"); err != nil {
		// Display error to user.
		switch err.(AuthError).Kind() {
		case AuthErrorCannotConnectDB:
			fmt.Println("Cannot connect to database")

		case AuthErrorUserNotFound:
			fmt.Println("User not found")

		case AuthErrorInvalidPassword:
			fmt.Println("Invalid password")
		}
	}
	fmt.Printf("\n")

	if err := Auth("invalid", "root", "root"); err != nil {
		// Logging.
		fmt.Println("--------------------")
		fmt.Printf("%v\n\n", err)

		cause := errors.Cause(err)

		// Logging root cause.
		fmt.Println("--------------------")
		fmt.Printf("%v\n\n", cause)

		// Logging with stack trace.
		fmt.Println("--------------------")
		fmt.Printf("%+v\n\n", errors.Cause(err))
	}
}
