package main

import (
	"fmt"

	"github.com/sirupsen/logrus"
	"github.com/spf13/cobra"

	"examples/go/urlshortener/db"
	"examples/go/urlshortener/db/mongodb"
	"examples/go/urlshortener/db/redis"
)

func main() {
	redisCfg := redis.Config{"127.0.0.1:6379", 0}
	mongodbCfg := mongodb.Config{"127.0.0.1:6379", "0"}

	dbCfg := interface{}{&redisCfg}

	logrus.SetLevel(logrus.DebugLevel)
	logrus.SetFormatter(&logrus.TextFormatter{
		FullTimestamp: true,
	})

	var cmdAdd = &cobra.Command{
		Use:   "add <url>",
		Short: "Shorten URLs",
		Args:  cobra.MinimumNArgs(1),
		Run: func(cmd *cobra.Command, args []string) {
			client, err := db.NewClient(dbCfg)
			if err != nil {
				fmt.Println(err)
				return
			}
			defer client.Close()

			for _, url := range args {
				if urlinfo, err := client.AddUrl(url); err != nil {
					fmt.Println(err)
				} else {
					fmt.Println(urlinfo)
				}
			}
		},
	}

	var cmdDel = &cobra.Command{
		Use:   "del <url>",
		Short: "Delete URLs",
		Args:  cobra.MinimumNArgs(1),
		Run: func(cmd *cobra.Command, args []string) {
			client, err := db.NewClient(dbCfg)
			if err != nil {
				fmt.Println(err)
				return
			}
			defer client.Close()

			for _, url := range args {
				if err := client.DelUrl(url); err != nil {
					fmt.Println(err)
				}
			}
		},
	}

	var cmdLst = &cobra.Command{
		Use:   "lst",
		Short: "List all URLs",
		Args:  cobra.NoArgs,
		Run: func(cmd *cobra.Command, args []string) {
			client, err := db.NewClient(dbCfg)
			if err != nil {
				fmt.Println(err)
				return
			}
			defer client.Close()

			lst, err := client.UrlLst()
			if err != nil {
				fmt.Println(err)
				return
			}

			for _, v := range lst {
				fmt.Println(v)
			}
		},
	}

	var rootCmd = &cobra.Command{}
	rootCmd.AddCommand(cmdAdd, cmdDel, cmdLst)
	rootCmd.Execute()
}
