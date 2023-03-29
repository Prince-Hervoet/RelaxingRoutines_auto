package core

import "sync"

// 缓冲区
type RingLightBuffer struct {
	capacity     int32
	current      int32
	content      []byte
	writePointer int32
	readPointer  int32
	isReading    bool
	hold         *sync.Mutex
}

func newLightBuffer(capacity int32) *RingLightBuffer {
	return &RingLightBuffer{capacity, 0, make([]byte, capacity), 0, 0, true, &sync.Mutex{}}
}

func (that *RingLightBuffer) Clear() {
	if that.isReading {
		return
	}
	that.writePointer = 0
	that.readPointer = 0
}

func (that *RingLightBuffer) write(bytes []byte) int8 {
	if that.isReading {
		return -1
	} else if that.current+int32(len(bytes)) > that.capacity {
		return 0
	}
	for i := int32(0); i < int32(len(bytes)); i++ {
		that.content[that.writePointer] = bytes[i]
		that.writePointer += 1
		if that.writePointer == that.capacity {
			that.writePointer = 0
		}
	}
	that.current += int32(len(bytes))
	return 1
}

func (that *RingLightBuffer) read(size int32) []byte {
	if !that.isReading || that.current == 0 || size == 0 {
		return make([]byte, 0)
	}
	min := int32Min(size, that.current)
	temp := make([]byte, min)
	for i := int32(0); i < int32(min); i++ {
		temp[0] = that.content[that.readPointer]
		that.readPointer += 1
		if that.readPointer == that.capacity {
			that.readPointer = 0
		}
	}
	that.current -= min
	return temp
}

func int32Min(a, b int32) int32 {
	if a > b {
		return b
	}
	return a
}
