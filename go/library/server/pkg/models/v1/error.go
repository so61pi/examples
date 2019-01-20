package v1

type Error struct {
	Code    int32  `json:"code"`
	Message string `json:"message"`
}
