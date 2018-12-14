package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"

	"examples/go/urlshortener/db/client"
	"examples/go/urlshortener/db/client/mongodb"
	"examples/go/urlshortener/db/client/redis"
)

func main() {
	redisCfg := redis.Config{"127.0.0.1:6379", 0}
	mongodbCfg := mongodb.Config{"127.0.0.1:6379", "0"}

	var dbCfg interface{}
	if true {
		dbCfg = &redisCfg
	} else {
		dbCfg = &mongodbCfg
	}

	logrus.SetLevel(logrus.DebugLevel)
	logrus.SetFormatter(&logrus.TextFormatter{
		FullTimestamp: true,
	})

	router := gin.Default()
	router.GET("/:url", func(c *gin.Context) {
		url := c.Param("url")
		logrus.WithFields(log.Fields(log.WebServer, url)).Debug("process url")

		client, err := client.NewClient(dbCfg)
		if err != nil {
			logrus.WithFields(log.Fields(log.Redis, err)).Debug("cannot access redis server")
			c.String(http.StatusInternalServerError, "Internal Server Error")
			return
		}
		defer client.Close()

		if urlInfo, err := client.GetUrl(url); err == nil && !urlInfo.Deleted {
			logrus.WithFields(log.Fields(log.WebServer, urlInfo.LongUrl)).Debug("redirecting")
			c.Redirect(http.StatusMovedPermanently, urlInfo.LongUrl)
			client.HitUrl(url)
		} else {
			logrus.WithFields(log.Fields(log.WebServer, url)).Debug("invalid url")
			c.String(http.StatusNotFound, "Page Not Found")
		}
	})
	router.Run(":8088")
}
