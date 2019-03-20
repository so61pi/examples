package main

import (
	"os"

	"github.com/sirupsen/logrus"
)

func init() {
	if true {
		logrus.SetFormatter(&logrus.TextFormatter{
			// DisableColors: true,
			// FullTimestamp: true,
		})
	} else {
		// Log as JSON instead of the default ASCII formatter.
		logrus.SetFormatter(&logrus.JSONFormatter{})
	}

	// Output to stdout instead of the default stderr.
	// Can be any io.Writer, see below for File example.
	logrus.SetOutput(os.Stdout)

	// Only log the warning severity or above.
	logrus.SetLevel(logrus.InfoLevel)
}

func main() {
	logrus.WithFields(logrus.Fields{
		"animal": "walrus",
		"size":   10,
	}).Info("A group of walrus emerges from the ocean")

	logrus.WithFields(logrus.Fields{
		"omg":    true,
		"number": 122,
	}).Warn("The group's number increased tremendously!")

	// This will end the program after logging.
	// logrus.WithFields(logrus.Fields{
	// 	"omg":    true,
	// 	"number": 100,
	// }).Fatal("The ice breaks!")

	// A common pattern is to re-use fields between logging statements by re-using
	// the logrus.Entry returned from WithFields()
	contextLogger := logrus.WithFields(logrus.Fields{
		"common": "this is a common field",
		"other":  "I also should be logged always",
	})

	contextLogger.Info("I'll be logged with common and other field")
	contextLogger.Info("Me too")
}
