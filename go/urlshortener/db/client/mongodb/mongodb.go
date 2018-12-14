package mongodb

import (
	"fmt"
	"math"
	"net/url"

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
			Key:        []string{"shorturl"},
			Unique:     true,
			DropDups:   false,
			Background: true,
		}
		if err := client.DB(cfg.Db).C("URL").EnsureIndex(index); err != nil {
			logrus.WithFields(log.Fields(log.Database, err)).Debug("ensure index failed")
			client.Close()
			return nil, err
		}

		return &mongodbSess{client, cfg.Db}, nil
	}
}

type data struct {
	Counter int `bson:"counter"`
}

func (sess *mongodbSess) AddUrl(longUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("AddUrl"))

	if _, err := url.ParseRequestURI(longUrl); err != nil {
		return nil, err
	}

	s := sess.client.Clone()
	defer s.Close()

	incCounter := func() (*data, error) {
		c := s.DB(sess.db).C("DATA")

		change := mgo.Change{
			Update:    bson.M{"$inc": bson.M{"counter": 1}},
			Upsert:    true,
			ReturnNew: true,
		}

		d := &data{}
		if _, err := c.Find(bson.M{}).Apply(change, d); err != nil {
			logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot update counter")
			return nil, err
		} else {
			logrus.WithFields(log.Fields(log.Database, err)).Debug("counter updated successfully")
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
		logrus.WithFields(log.Fields(log.Other, shortUrl)).Debug("URL generated")

		urlInfo := model.NewUrlInfo(shortUrl, longUrl, 0, false)
		if err := c.Insert(urlInfo); err != nil {
			logrus.WithFields(log.Fields(log.Database, err)).Debug("URL cannot be inserted")
			if mgo.IsDup(err) {
				logrus.WithFields(log.Fields(log.Database, err)).Debug("generated URL is duplicated, re-try")
				continue
			}
			return nil, err
		}
		logrus.WithFields(log.Fields(log.Database, err)).Debug("URL inserted successfully")
		return urlInfo, nil
	}
}

func (sess *mongodbSess) GetUrl(shortUrl string) (*model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("GetUrl"))
	s := sess.client.Clone()
	defer s.Close()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"shorturl": shortUrl}).One(urlInfo); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot retrieve URL")
		return nil, err
	}

	logrus.WithFields(log.Fields(log.Database, urlInfo)).Debug("URL retrieved successfully")
	return urlInfo, nil
}

func (sess *mongodbSess) HitUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("HitUrl"))
	s := sess.client.Clone()
	defer s.Close()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"shorturl": shortUrl}).One(urlInfo); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot get URL")
		return err
	}
	logrus.WithFields(log.Fields(log.Database, urlInfo)).Debug("URL retrieved successfully")

	change := mgo.Change{
		Update:    bson.M{"$inc": bson.M{"hit": 1}},
		ReturnNew: true,
	}

	if _, err := c.Find(bson.M{"shorturl": shortUrl}).Apply(change, urlInfo); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot update hit")
		return err
	}

	logrus.WithFields(log.Fields(log.Database, urlInfo)).Debug("hit updated successfully")
	return nil
}

func (sess *mongodbSess) DelUrl(shortUrl string) error {
	defer log.FnExit(log.FnEnter("DelUrl"))
	s := sess.client.Clone()
	defer s.Close()

	c := s.DB(sess.db).C("URL")

	urlInfo := &model.UrlInfo{}
	if err := c.Find(bson.M{"shorturl": shortUrl}).One(urlInfo); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot get URL")
		return err
	}
	logrus.WithFields(log.Fields(log.Database, urlInfo)).Debug("URL retrieved successfully")

	change := mgo.Change{
		Update:    bson.M{"$set": bson.M{"deleted": true}},
		ReturnNew: true,
	}

	if info, err := c.Find(bson.M{"shorturl": shortUrl}).Apply(change, urlInfo); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot mark URL as deleted")
		return err
	} else {
		logrus.WithFields(log.Fields(log.Database, fmt.Sprintf("%#v", info))).Debug("URL marked as deleted")
		return nil
	}
}

func (sess *mongodbSess) UrlLst() ([]model.UrlInfo, error) {
	defer log.FnExit(log.FnEnter("UrlLst"))
	s := sess.client.Clone()
	defer s.Close()

	c := s.DB(sess.db).C("URL")

	all := []model.UrlInfo{}
	if err := c.Find(bson.M{}).All(&all); err != nil {
		logrus.WithFields(log.Fields(log.Database, err)).Debug("cannot get URLs")
		return nil, err
	}
	logrus.WithFields(log.Fields(log.Database, all)).Debug("URLs retrieved successfully")
	return all, nil
}

func (sess *mongodbSess) Close() {
	sess.client.Close()
}
