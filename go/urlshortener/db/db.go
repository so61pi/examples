package db

type UrlInfo struct {
    ShortUrl string
    LongUrl string
    Hit uint64
    Deleted bool
}

func NewUrlInfo(shortUrl string, longUrl string, hit uint64, deleted bool) *UrlInfo {
    return &UrlInfo{
        ShortUrl: shortUrl,
        LongUrl: longUrl,
        Hit: hit,
        Deleted: deleted,
    }
}

type DB interface {
    AddUrl(url string) (*UrlInfo, error)
    DelUrl(url string) error
    GetUrl(url string) (*UrlInfo, error)
    UrlLst() ([]UrlInfo, error)
}
