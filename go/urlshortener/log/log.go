package log

import (
	"github.com/sirupsen/logrus"
)

type Topic string

const (
	WebServer = Topic("webserver")
	Redis     = Topic("redis")
	Function  = Topic("function")

	Other = Topic("other")
)

func Fields(topic Topic, val interface{}) logrus.Fields {
	return logrus.Fields{
		"topic": topic,
		"value": val,
	}
}

func FnEnter(fn string) string {
	logrus.WithFields(Fields(Function, fn)).Debug("enter function")
	return fn
}

func FnExit(fn string) {
	logrus.WithFields(Fields(Function, fn)).Debug("exit function")
}
