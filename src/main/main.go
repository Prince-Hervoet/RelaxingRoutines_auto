package main

import (
	"fmt"
	"light_logger/src/util"
)

func main() {
	f := util.OpenFile("D:\\myprojects\\light_logger\\test\\log1.txt")
	fmt.Println(f)
	util.WriteStringToFile(f, "1231222222222")
	util.CloseFile(f)
}
