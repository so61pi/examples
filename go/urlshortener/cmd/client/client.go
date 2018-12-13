package main

import (
	"fmt"

	"github.com/sirupsen/logrus"
	"github.com/spf13/cobra"

	"examples/go/urlshortener/db/redis"
)

func main() {
	const redisserver = "127.0.0.1:6379"

	logrus.SetLevel(logrus.DebugLevel)
	logrus.SetFormatter(&logrus.TextFormatter{
		FullTimestamp: true,
	})

	var cmdAdd = &cobra.Command{
		Use:   "add <url>",
		Short: "Shorten a URL",
		Args:  cobra.ExactArgs(1),
		Run: func(cmd *cobra.Command, args []string) {
			client, err := redis.NewClient(redisserver, 0)
			if err != nil {
				fmt.Println(err)
				return
			}
			defer client.Close()

			urlinfo, err := client.AddUrl(args[0])
			if err != nil {
				fmt.Println(err)
				return
			}

			fmt.Println(urlinfo)
		},
	}

	var cmdDel = &cobra.Command{
		Use:   "del <url>",
		Short: "Delete a URL",
		Args:  cobra.ExactArgs(1),
		Run: func(cmd *cobra.Command, args []string) {
			client, err := redis.NewClient(redisserver, 0)
			if err != nil {
				fmt.Println(err)
				return
			}
			defer client.Close()

			if err := client.DelUrl(args[0]); err != nil {
				fmt.Println(err)
				return
			}
		},
	}

	var cmdLst = &cobra.Command{
		Use:   "lst",
		Short: "List all URLs",
		Args:  cobra.NoArgs,
		Run: func(cmd *cobra.Command, args []string) {
			client, err := redis.NewClient(redisserver, 0)
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
