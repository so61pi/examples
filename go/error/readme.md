To create new errors, we have 3 ways:

- Use `errors.New` from `github.com/pkg/errors` or related functions.
  - In this way, errors already record stack trace.
  - This should be used sparingly otherwise we cannot differentiate errors anymore.
  - **Note that the built-in `errors.New` doesn't record stack trace information.** We can use `errors.WithStack` to add stack trace info for existing errors.

- Create a new error struct that wrap around other errors.
```go
type MyErrorKind int
const (
    MyError1 MyErrorKind = itoa
    MyError2
    MyError3
)

type MyError struct {
    kind MyErrorKind
    cause error
    
    // ... additional info
}

// NewMyError constructs a new instance of MyError.
//
// We don't use error as the return type because caller can do it.
func NewMyError(kind MyErrorKind, cause error) MyError {
    return MyError{
        kind: kind,
        cause: cause,
    }
}

// Kind returns the error kind so caller and give a user-friendly message
// (which can be in other languages than English).
func (err *MyError) Kind() MyErrorKind {
    return err.kind
}

// Error returns a short description which doesn't need to be user-friendly.
func (err *MyError) Error() string {
    // TODO: Format your error here.
}

// Cause returns the underlying error of this error.
//
// This function implement causer interface so we can use errors.Cause
// to get the root cause which is usually created by errors.New or
// errors.Errorf
func (err *MyError) Cause() {
    return err.cause
}
```

- Create a new independent error (does not wrap other errors).
```go
type MyParseIntError struct {
    err error
}

func NewMyParseIntError(input string) MyParseIntError {
    return MyParseIntError{
        err: errors.Errorf("invalid int number '%s'", input),
    }
}

func (err *MyError) Error() string {
    return err.err.Error()
}

func (err *MyError) Cause() {
    return err.cause
}
```
