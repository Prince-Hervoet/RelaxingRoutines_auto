package core

import "light_logger/src/util"

type Buffer struct {
	capacity int32
	content  []byte
	size     int32
}

func NewBuffer(capacity int32) *Buffer {
	return &Buffer{capacity: capacity, content: make([]byte, capacity), size: 0}
}

func (that *Buffer) Write(content string) int32 {
	bs := util.StringToByteSlice(content)
	if int32(len(bs))+that.size > that.capacity {
		return -1
	}
	for i := int32(0); i < int32(len(bs)); i++ {
		that.content[that.size] = bs[i]
		that.size += 1
	}
	return 1
}

func (that *Buffer) OutputToFile(path string) {
	if that.size == 0 {
		return
	}
	file := util.OpenFile(path)
	if file != nil {
		defer util.CloseFile(file)
		ss := util.ByteSliceToString(that.content[0:that.size])
		util.WriteStringToFile(file, ss)
		that.content = make([]byte, that.capacity)
		that.size = 0
	}

}
