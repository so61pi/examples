package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"

	"examples/go/urlshortener/db/redis"
	"examples/go/urlshortener/log"
)

func main() {
	const redisserver = "127.0.0.1:6379"

	logrus.SetLevel(logrus.DebugLevel)
	logrus.SetFormatter(&logrus.TextFormatter{
		FullTimestamp: true,
	})

	router := gin.Default()
	router.GET("/:url", func(c *gin.Context) {
		url := c.Param("url")
		logrus.WithFields(log.Fields(log.WebServer, url)).Debug("process url")

		client, err := redis.NewClient(redisserver, 0)
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
