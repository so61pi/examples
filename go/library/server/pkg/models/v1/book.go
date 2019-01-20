package v1

type Book struct {
	Id          int64  `json:"id"`
	Name        string `json:"name"`
	Description string `json:"description,omitempty"`
	Author      string `json:"author,omitempty"`
	Publisher   string `json:"publisher,omitempty"`
}
