package mongodb

import (
	// "errors"
	"math"
	"net/url"
	// "strings"

	"github.com/sirupsen/logrus"
	"gopkg.in/mgo.v2"
	"gopkg.in/mgo.v2/bson"

	"examples/go/urlshortener/db"
	"examples/go/urlshortener/log"
	"examples/go/urlshortener/model"
	"examples/go/urlshortener/util"
)

type mongodbSess struct {
	client *mgo.Session
	db     string
}

type Config struct {
	Addr string
	Db   string
}

func NewClient(cfg Config) (db.DB, error) {
	if client, err := mgo.Dial(cfg.Addr); err != nil {
		return nil, err
	} else {
		index := mgo.Index{
			Key:        []string{"ShortUrl"},
			Unique:     true,
			DropDups:   false,
			Background: true,
		}
		if err := client.DB(cfg.Db).C("URL").EnsureIndex(index); err != nil {
			client.Close()
			return nil, err
		}

		return &mongodbSess{client, cfg.Db}, nil
	}
}

type data struct {
	Counter int
}

func (sess *mongodbSess) AddUrl(longUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("AddUrl"))

	if _, err := url.ParseRequestURI(longUrl); err != nil {
		return nil, err
	}

	s := sess.client.Clone()
	defer s.Clone()

	incCounter := func() (*data, error) {
		c := s.DB(sess.db).C("DATA")

		change := mgo.Change{
			Update:    bson.M{"$inc": bson.M{"Counter": 1}},
			ReturnNew: true,
		}

		d := &data{}
		if _, err := c.Find(bson.M{}).Apply(change, d); err != nil {
			return nil, err
		} else {
			return d, nil
		}
	}

	counter, err := incCounter()
	if err != nil {
		return nil, err
	}

	length := int(math.Floor(math.Log10(float64(counter.Counter))) + 1)
	if length < 4 {
		length = 4
	}

	c := s.DB(sess.db).C("URL")
	for {
		shortUrl, err := util.GenerateRandomString(length)
		if err != nil {
			return nil, err
		}
		logrus.WithFields(log.Fields(log.Other, shortUrl)).Debug("url generated")

		urlInfo := model.NewUrlInfo(shortUrl, longUrl, 0, false)
		if err := c.Insert(urlInfo); err != nil {
			if mgo.IsDup(err) {
				continue
			}
			return nil, err
		}
		return urlInfo, nil
	}
}

func (sess *mongodbSess) GetUrl(shortUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("GetUrl"))
	s := sess.client.Clone()
	defer s.Clone()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"ShortUrl": shortUrl}).One(urlInfo); err != nil {
		return nil, err
	}
	return urlInfo, nil
}

func (sess *mongodbSess) HitUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("HitUrl"))
	s := sess.client.Clone()
	defer s.Clone()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"ShortUrl": shortUrl}).One(urlInfo); err != nil {
		return err
	}

	change := mgo.Change{
		Update:    bson.M{"$inc": bson.M{"Hit": 1}},
		ReturnNew: true,
	}

	if _, err := c.Find(bson.M{"ShortUrl": shortUrl}).Apply(change, urlInfo); err != nil {
		return err
	}
	return nil
}

func (sess *mongodbSess) DelUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("HitUrl"))
	s := sess.client.Clone()
	defer s.Clone()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"ShortUrl": shortUrl}).One(urlInfo); err != nil {
		return err
	}

	change := mgo.Change{
		Update:    bson.M{"$set": bson.M{"Deleted": true}},
		ReturnNew: true,
	}

	if _, err := c.Find(bson.M{"ShortUrl": shortUrl}).Apply(change, urlInfo); err != nil {
		return err
	} else {
		return nil
	}
}

func (sess *mongodbSess) UrlLst() ([]model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("UrlLst"))
	s := sess.client.Clone()
	defer s.Clone()

	c := s.DB(sess.db).C("URL")

	all := []model.UrlInfo{}
	if err := c.Find(bson.M{}).All(&all); err != nil {
		return nil, err
	}
	return all, nil
}

func (sess *mongodbSess) Close() {
	sess.Close()
}
