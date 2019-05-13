package tfuncparam

type GetUsernameFn func(uint64) (string, error)

func IsRoot(id uint64, GetUsername GetUsernameFn) (bool, error) {
	if name, err := GetUsername(id); err != nil {
		return false, err
	} else {
		return name == "root", nil
	}
}
