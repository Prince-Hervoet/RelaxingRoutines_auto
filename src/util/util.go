package util

import (
	"bufio"
	"fmt"
	"os"
	"unsafe"
)

func OpenFile(path string) *os.File {
	file, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE, 0666)
	if err != nil {
		fmt.Println(err.Error())
		return nil
	}
	return file
}

func WriteStringToFile(file *os.File, log string) bool {
	if file == nil {
		return false
	}
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

func StringTobyteSlice(s string) []byte {
	tmp1 := (*[2]uintptr)(unsafe.Pointer(&s))
	tmp2 := [3]uintptr{tmp1[0], tmp1[1], tmp1[1]}
	return *(*[]byte)(unsafe.Pointer(&tmp2))
}

func ByteSliceToString(bytes []byte) string {
	return *(*string)(unsafe.Pointer(&bytes))
}

func Int32Min(a, b int32) int32 {
	if a < b {
		return a
	}
	return b
}
