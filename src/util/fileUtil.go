package util

import (
	"bufio"
	"fmt"
	"os"
)

func WriteStringToFile(path string, log string) bool {
	file, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE, 0666)
	if err != nil {
		return false
	}
	defer CloseFile(file)
	write := bufio.NewWriter(file)
	write.WriteString(log)
	write.Flush()
	return true
}

func CloseFile(file *os.File) {
	if file != nil {
		err := file.Close()
		if err != nil {
			fmt.Println(err.Error())
		}
	}
}
