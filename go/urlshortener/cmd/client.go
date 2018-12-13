package main

import (
    "fmt"
    // "os"
    // "strings"
    "github.com/spf13/cobra"
    "examples/go/urlshortener/db/redis"
)

func main() {
    var cmdAdd = &cobra.Command{
        Use:   "add <url>",
        Short: "Shorten a URL",
        Args: cobra.ExactArgs(1),
        Run: func(cmd *cobra.Command, args []string) {
            db, err := redis.NewClient("127.0.0.1:6379", 0)
            if err != nil {
                fmt.Println(err)
                return
            }
            urlinfo, err := db.AddUrl(args[0])
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
        Args: cobra.ExactArgs(1),
        Run: func(cmd *cobra.Command, args []string) {
            db, err := redis.NewClient("127.0.0.1:6379", 0)
            if err != nil {
                fmt.Println(err)
                return
            }
            if err := db.DelUrl(args[0]); err != nil {
                fmt.Println(err)
                return
            }
        },
    }

    var cmdLst = &cobra.Command{
        Use:   "lst",
        Short: "List all URLs",
        Args: cobra.NoArgs,
        Run: func(cmd *cobra.Command, args []string) {
            db, err := redis.NewClient("127.0.0.1:6379", 0)
            if err != nil {
                fmt.Println(err)
                return
            }

            lst, err := db.UrlLst()
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
