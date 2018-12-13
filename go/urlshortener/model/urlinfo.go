package model

import (
	"fmt"
	"strconv"
)

type UrlInfo struct {
	ShortUrl string
	LongUrl  string
	Hit      uint64
	Deleted  bool
}

func NewUrlInfo(shortUrl string, longUrl string, hit uint64, deleted bool) *UrlInfo {
	return &UrlInfo{
		ShortUrl: shortUrl,
		LongUrl:  longUrl,
		Hit:      hit,
		Deleted:  deleted,
	}
}

func (urlInfo UrlInfo) String() string {
	return fmt.Sprintf("%v -> %v | hit : %v | deleted : %v",
		urlInfo.ShortUrl,
		urlInfo.LongUrl,
		urlInfo.Hit,
		urlInfo.Deleted)
}

func (urlInfo *UrlInfo) FromMap(kv map[string]string) error {
	urlInfo.LongUrl = kv["url"]

	if s, err := strconv.Atoi(kv["deleted"]); err == nil {
		urlInfo.Deleted = uint64(s) > 0
	} else {
		return err
	}

	if s, err := strconv.Atoi(kv["hit"]); err == nil {
		urlInfo.Hit = uint64(s)
	} else {
		return err
	}

	return nil
}
