package main

import (
	"light_logger/src/core"
	"strconv"
)

func main() {
	// f := util.OpenFile("D:\\myprojects\\light_logger\\test\\log1.txt")
	ll := core.NewLightLogger(100, 4096)
	ll.SetDir("D:\\myprojects\\light_logger\\test\\")
	ll.Start()
	for i := 0; i < 1000000; i++ {
		ll.Info(strconv.Itoa(i) + ": " + "qweqweqweqweqwe")
	}
	for {
	}
}
