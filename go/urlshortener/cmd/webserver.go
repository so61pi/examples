package main

import (
    "net/http"
)

func main() {
    http.Handle("/foo", fooHandler)

    http.HandleFunc("/bar", func(w http.ResponseWriter, r *http.Request) {
        fmt.Fprintf(w, "Hello, %q", html.EscapeString(r.URL.Path))
    })
}
