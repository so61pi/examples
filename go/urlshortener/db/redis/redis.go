package redis

import (
	"errors"
	"math"
	"net/url"
	"strings"

	"github.com/go-redis/redis"
	"github.com/sirupsen/logrus"

	"examples/go/urlshortener/db"
	"examples/go/urlshortener/log"
	"examples/go/urlshortener/model"
	"examples/go/urlshortener/util"
)

type redisSess struct {
	client *redis.Client
}

func NewClient(addr string, db int) (db.DB, error) {
	client := redis.NewClient(&redis.Options{
		Addr: addr,
		DB:   db,
	})
	_, err := client.Ping().Result()
	if err != nil {
		return nil, err
	}

	return &redisSess{client}, nil
}

func (r *redisSess) Close() {
	r.client.Close()
}

func (r *redisSess) AddUrl(longUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("AddUrl"))

	if _, err := url.ParseRequestURI(longUrl); err != nil {
		return nil, err
	}

	tryAdd := func(r *redisSess, key string, longUrl string) (bool, error) {
		defer log.FnExit(log.FnEnter("tryAdd"))

		ok, err := r.client.HSetNX(key, "url", longUrl).Result()
		if err != nil {
			return false, err
		}

		if !ok {
			logrus.WithFields(log.Fields(log.Redis, key)).Debug("key already exists")
			return false, nil
		}

		if _, err := r.client.HMSet(key, map[string]interface{}{"hit": 0, "deleted": 0}).Result(); err != nil {
			return false, err
		}

		logrus.WithFields(log.Fields(log.Redis, key)).Debug("url added successfully")
		return true, nil
	}

	counter, err := r.client.Incr("counter").Result()
	if err != nil {
		return nil, err
	}
	logrus.WithFields(log.Fields(log.Redis, counter)).Debug("current value of counter")

	length := int(math.Floor(math.Log10(float64(counter))) + 1)
	if length < 4 {
		length = 4
	}

	for {
		shortUrl, err := util.GenerateRandomString(length)
		if err != nil {
			return nil, err
		}
		logrus.WithFields(log.Fields(log.Other, shortUrl)).Debug("url generated")

		key := "url:" + shortUrl
		success, err := tryAdd(r, key, longUrl)
		if err != nil {
			return nil, err
		}

		if !success {
			logrus.WithFields(log.Fields(log.Redis, shortUrl)).Debug("url cannot be added, re-try")
			continue
		}
		return model.NewUrlInfo(shortUrl, longUrl, 0, false), nil
	}
}

func (r *redisSess) GetUrl(shortUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("GetUrl"))

	logrus.WithFields(log.Fields(log.Redis, shortUrl)).Debug("get data of url")

	key := "url:" + shortUrl
	mapCmd, err := r.client.HGetAll(key).Result()
	if err != nil {
		return nil, err
	}

	logrus.WithFields(log.Fields(log.Redis, mapCmd)).Debug("data of url")

	urlInfo := &model.UrlInfo{}
	urlInfo.ShortUrl = shortUrl
	if err := urlInfo.FromMap(mapCmd); err != nil {
		return nil, err
	}

	return urlInfo, nil
}

func (r *redisSess) HitUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("HitUrl"))

	key := "url:" + shortUrl
	v, err := r.client.HIncrBy(key, "hit", 1).Result()

	if err == nil {
		logrus.WithFields(log.Fields(log.Redis, v)).Debug("new number of hits")
	}
	return err
}

func (r *redisSess) DelUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("DelUrl"))

	key := "url:" + shortUrl

	var markAsDeleted func(key string) error
	markAsDeleted = func(key string) error {
		err := r.client.Watch(func(tx *redis.Tx) error {
			if exists, err := tx.Exists(key).Result(); err != nil {
				tx.Unwatch(key)
				return err
			} else if exists == 0 {
				tx.Unwatch(key)
				logrus.WithFields(log.Fields(log.Redis, shortUrl)).Debug("url not found in database")
				return errors.New("url not found")
			}

			_, err := tx.Pipelined(func(pipe redis.Pipeliner) error {
				pipe.HSet(key, "deleted", 1)
				return nil
			})
			return err
		}, key)

		if err == redis.TxFailedErr {
			logrus.WithFields(log.Fields(log.Redis, err)).Debug("transaction failed, re-try")
			return markAsDeleted(key)
		}
		return err
	}

	return markAsDeleted(key)
}

func (r *redisSess) UrlLst() ([]model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("UrlLst"))

	key := "url:*"
	keys, err := r.client.Keys(key).Result()
	if err != nil {
		return nil, err
	}

	logrus.WithFields(log.Fields(log.Redis, keys)).Debug("retrieved urls")

	urls := []model.UrlInfo{}
	for _, v := range keys {
		logrus.WithFields(log.Fields(log.Redis, v)).Debug("getting data for key")
		mapCmd, err := r.client.HGetAll(v).Result()
		if err != nil {
			logrus.WithFields(log.Fields(log.Redis, err)).Debug("error while getting data for key")
			return nil, err
		}

		urlInfo := model.UrlInfo{ShortUrl: strings.TrimPrefix(v, "url:")}
		if err := urlInfo.FromMap(mapCmd); err != nil {
			logrus.WithFields(log.Fields(log.Other, err)).Debug("cannot translate db data to url info")
			return nil, err
		}

		logrus.WithFields(log.Fields(log.Other, urlInfo)).Debug("getting data for key successfully")
		urls = append(urls, urlInfo)
	}

	return urls, nil
}
