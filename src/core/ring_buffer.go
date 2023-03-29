package core

import "light_logger/src/util"

type RingBuffer struct {
	data        []byte
	writeCursor int32
	readCursor  int32
	size        int32
	capacity    int32
}

func NewRingBuffer(capacity int32) *RingBuffer {
	rb := &RingBuffer{}
	rb.capacity = capacity
	rb.data = make([]byte, capacity)
	rb.size = 0
	rb.writeCursor = 0
	rb.readCursor = 0
	return rb
}

func (that *RingBuffer) Write(content string) int8 {
	bs := util.StringTobyteSlice(content)
	if int32(len(bs))+that.size > that.capacity {
		return 0
	}
	for i := 0; i < len(bs); i++ {
		that.data[that.writeCursor] = bs[i]
		that.writeCursor += 1
		if that.writeCursor == that.capacity {
			that.writeCursor = 0
		}
	}
	that.size += int32(len(bs))
	return 1
}

func (that *RingBuffer) Read(need []byte) int32 {
	if len(need) == 0 {
		return 0
	}
	min := util.Int32Min(int32(len(need)), that.size)
	for i := int32(0); i < min; i++ {
		need[i] = that.data[that.readCursor]
		that.readCursor += 1
		if that.readCursor == that.capacity {
			that.readCursor = 0
		}
	}
	that.size -= int32(min)
	return int32(min)
}

func (that *RingBuffer) Reset() {
	that.size = 0
	that.readCursor = 0
	that.writeCursor = 0
}
