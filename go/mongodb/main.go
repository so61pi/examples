package main

import (
	"fmt"
	"time"

	"github.com/globalsign/mgo"
)

type Person struct {
	ID   int64  `bson:"id"`
	Name string `bson:"name"`
}

func NewPerson(id int64, name string) Person {
	return Person{
		ID:   id,
		Name: name,
	}
}

func main() {
	dialInfo := &mgo.DialInfo{
		Addrs:    []string{"127.0.0.1:27017"},
		Timeout:  3 * time.Second,
		Username: "root",
		Password: "example",
	}

	sess, err := mgo.DialWithInfo(dialInfo)
	if err != nil {
		fmt.Println("Cannot connect to mongodb server.")
		return
	}
	defer sess.Close()

	dbName := "testdb"
	colName := "testcol"
	col := sess.DB(dbName).C(colName)
	if err := UseCollection(col); err != nil {
		fmt.Printf("Failed with error: '%s'\n", err)
	} else {
		fmt.Println("Success.")
	}
}

func UseCollection(col *mgo.Collection) error {
	return col.Insert(NewPerson(1, "1"))
}
