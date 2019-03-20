package main

import (
	"fmt"
	"os"
	"reflect"
	"sort"
	"strings"

	"github.com/spf13/viper"
)

func ConfigFromEnv(conf interface{}) error {
	viper.AutomaticEnv()
	viper.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))
	bindEnv(conf)

	if err := viper.Unmarshal(conf); err != nil {
		return err
	}

	missingKeys := []string{}
	keys := viper.AllKeys()
	for _, v := range keys {
		if viper.Get(v) == nil {
			missingKeys = append(missingKeys, strings.Replace(v, ".", "_", -1))
		}
	}

	if len(missingKeys) > 0 {
		sort.Strings(missingKeys)
		return fmt.Errorf("missing environment variables: %v", strings.Join(missingKeys, " "))
	}

	return nil
}

func bindEnv(conf interface{}) {
	if pt := reflect.TypeOf(conf).Kind(); pt != reflect.Ptr {
		panic(fmt.Sprintf("invalid type, should be pointer instead of %v", pt))
	}

	t := reflect.Indirect(reflect.ValueOf(conf)).Type()
	if t.Kind() != reflect.Struct {
		panic(fmt.Sprintf("invalid type, should be struct instead of %v", t.Kind()))
	}

	bindEnvImpl(t)
}

func bindEnvImpl(ptype reflect.Type, parts ...string) {
	for i := 0; i < ptype.NumField(); i++ {
		field := ptype.Field(i)
		tv, ok := field.Tag.Lookup("mapstructure")
		if !ok {
			panic(fmt.Sprintf("mapstructure is missing on %v", strings.Join(append(parts, field.Name), ".")))
		}
		switch field.Type.Kind() {
		case reflect.Struct:
			bindEnvImpl(field.Type, append(parts, tv)...)
		default:
			viper.BindEnv(strings.Join(append(parts, tv), "."))
		}
	}
}

type PersonInfo struct {
	FirstName string `mapstructure:"first_name"`
	LastName  string `mapstructure:"last_name"`
	Age       uint   `mapstructure:"age"`
}

type MyConfig struct {
	LogLevel string     `mapstructure:"log_level"`
	Person   PersonInfo `mapstructure:"person"`
}

func main() {
	os.Setenv("LOG_LEVEL", "Off")
	os.Setenv("PERSON_FIRST_NAME", "First")
	os.Setenv("PERSON_LAST_NAME", "Last")
	os.Setenv("PERSON_AGE", "20")

	var config MyConfig
	if err := ConfigFromEnv(&config); err != nil {
		fmt.Println(err)
	} else {
		fmt.Printf("%+v\n", config)
	}
}
