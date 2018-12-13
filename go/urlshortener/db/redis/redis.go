package redis

import (
    "fmt"
    "strconv"
    "github.com/go-redis/redis"
    "examples/go/urlshortener/db"
    "crypto/rand"
    "encoding/base64"
    "strings"
    "math"
    "net/url"
)


type redisSession struct {
    client *redis.Client
}

func NewClient(addr string, db int) (db.DB, error) {
    client := redis.NewClient(&redis.Options{
            Addr: addr,
            DB: db,
        })
    _, err := client.Ping().Result()
    if err != nil {
        return nil, err
    }

    return &redisSession{client}, nil
}

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


func (r *redisSession) AddUrl(longUrl string) (*db.UrlInfo, error) {
    if _, err := url.ParseRequestURI(longUrl); err != nil {
        return nil, err
    }

    tryAdd := func(r *redisSession, key string, longUrl string) (bool, error) {
        ok, err := r.client.HSetNX(key, "url", longUrl).Result()
        if err != nil {
            return false, err
        }

        if !ok {
            return false, nil
        }

        if _, err := r.client.HMSet(key, map[string]interface{}{"hit": 0, "deleted": 0}).Result(); err != nil {
            return false, err
        }
        return true, nil
    }

    counter, err := r.client.Incr("counter").Result()
    if err != nil {
        return nil, err
    }

    length := int(math.Floor(math.Log10(float64(counter))) + 1)
    if length < 4 {
        length = 4
    }

    for {
        shortUrl, err := GenerateRandomString(length)
        if err != nil {
            return nil, err
        }
        key := "url:" + shortUrl
        success, err := tryAdd(r, key, longUrl)
        if err != nil {
            return nil, err
        }
        if !success {
            continue
        }
        return db.NewUrlInfo(shortUrl, longUrl, 0, false), nil
    }
}

func (r *redisSession) DelUrl(shortUrl string) error {
    key := "url:" + shortUrl
    _, err := r.client.HSetNX(key, "deleted", 1).Result()
    return err
}


func mapStringToUrlInfo(mapCmd map[string]string, urlInfo *db.UrlInfo) error {
    fmt.Println(mapCmd)
    urlInfo.LongUrl = mapCmd["url"]

    if s, err := strconv.Atoi(mapCmd["deleted"]); err == nil {
        urlInfo.Deleted = uint64(s) > 0
    } else {
        return err
    }

    if s, err := strconv.Atoi(mapCmd["hit"]); err == nil {
        urlInfo.Hit = uint64(s)
    } else {
        return err
    }

    return nil
}

func (r *redisSession) GetUrl(shortUrl string) (*db.UrlInfo, error) {
    key := "url:" + shortUrl

    mapCmd, err := r.client.HGetAll(key).Result()
    if err != nil {
        return nil, err
    }

    urlInfo := &db.UrlInfo{}
    urlInfo.ShortUrl = shortUrl
    if err := mapStringToUrlInfo(mapCmd, urlInfo); err != nil {
        return nil, err
    }

    return urlInfo, nil
}

func (r *redisSession) UrlLst() ([]db.UrlInfo, error) {
    key := "url:*"
    keys, err := r.client.Keys(key).Result()
    if err != nil {
        return nil, err
    }

    urls := []db.UrlInfo{}
    for _, v := range keys {
        mapCmd, err := r.client.HGetAll(v).Result()
        if err != nil {
            continue
        }

        urlInfo := db.UrlInfo{ShortUrl : v}
        if err := mapStringToUrlInfo(mapCmd, &urlInfo); err != nil {
            return nil, err
        }
        urls = append(urls, urlInfo)
    }

    return urls, nil
}
