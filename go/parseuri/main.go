package main

import (
	"encoding/json"
	"fmt"
	"net/url"
)

func parse(uris []string) {
	fmt.Println("--==[ url.Parse ]==--")
	for i, uri := range uris {
		u, e := url.Parse(uri)
		b, _ := json.MarshalIndent(u, "", "    ")

		fmt.Printf("%v. %s( %s ) - (e = %v)\n", i, "url.Parse", uri, e)
		fmt.Println(string(b))
		fmt.Println()
	}
}

func parseRequestURI(uris []string) {
	fmt.Println("--==[ url.ParseRequestURI ]==--")
	for i, uri := range uris {
		u, e := url.ParseRequestURI(uri)
		b, _ := json.MarshalIndent(u, "", "    ")

		fmt.Printf("%v. %s( %s ) - (e = %v)\n", i, "url.ParseRequestURI", uri, e)
		fmt.Println(string(b))
		fmt.Println()
	}
}

func main() {
    // https://tools.ietf.org/html/rfc3986#appendix-A
	uris := []string{
		"example.com",         "example.com/v1",
		"mailto:example.com",  "mailto:example.com/v1",
		"https://example.com", "https://example.com/v1",

		"user@example.com",         "user@example.com/v1",
		"mailto:user@example.com",  "mailto:user@example.com/v1",
		"https://user@example.com", "https://user@example.com/v1",
	}
	parse(uris)
	parseRequestURI(uris)
}
