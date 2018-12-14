package log

import (
	"github.com/sirupsen/logrus"
)

type Topic string

const (
	WebServer = Topic("WEBSERVER")
	Database  = Topic("DB")
	Function  = Topic("FUNC")
	Other     = Topic("OTHER")
)

func Fields(topic Topic, val interface{}) logrus.Fields {
	return logrus.Fields{
		"topic": topic,
		"value": val,
	}
}

func FnEnter(fn string) string {
	logrus.WithFields(Fields(Function, fn)).Debug("ENTER>>>")
	return fn
}

func FnExit(fn string) {
	logrus.WithFields(Fields(Function, fn)).Debug("<<<EXIT")
}
