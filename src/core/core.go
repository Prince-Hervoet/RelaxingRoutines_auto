package core

import (
	"light_logger/src/util"
	"sync"
)

const (
	OUT_FILE    = 1
	OUT_CONSOLE = 2
)

const DEFAULT_BUFFER_SIZE = int32(65536)

type Logger interface {
	Info()
	Warn()
	Error()
	Debug()
}

type LightLogger struct {
	// 输出范围
	isOutToFile bool
	// 当前使用的缓冲区
	current *RingBuffer
	// 预备
	reserve  *RingBuffer
	reserveX []*RingBuffer
	// 最大缓冲区数量
	maxBufferCount int32
	// 当前缓冲区数量
	bufferCount int32
	// 文件目录路径
	dir string
	// 文件名
	fileName string
	// 队列
	cc       chan *RingBuffer
	capacity int32
	lock     *sync.Mutex
}

func NewLightLogger(capacity, maxBufferCount int32) *LightLogger {
	tl := &LightLogger{}
	tl.current = NewRingBuffer(capacity)
	tl.current = NewRingBuffer(capacity)
	tl.maxBufferCount = maxBufferCount
	tl.bufferCount = 2
	tl.capacity = capacity
	tl.cc = make(chan *RingBuffer, maxBufferCount)
	return tl
}

func (that *LightLogger) Start() {
	go func() {
		for buffer := range that.cc {
			that.persistent(buffer)

		}
	}()
}

func (that *LightLogger) Info() {

}

func (that *LightLogger) Warn() {
}

func (that *LightLogger) Error() {

}

func (that *LightLogger) Debug() {

}

func (that *LightLogger) exchangeBuffers() {
	that.lock.Lock()
	defer that.lock.Unlock()
	if that.current.size == 0 || (that.current.size < that.current.capacity && that.reserve == nil) {
		return
	}
	if that.reserve == nil {
		that.reserve = NewRingBuffer(that.capacity)
	}
	temp := that.current
	that.current = that.reserve
	that.reserve = temp
	that.cc <- that.reserve
	that.reserve = nil
}

func (that *LightLogger) persistent(rb *RingBuffer) {
	bs := rb.data[rb.readCursor : rb.writeCursor+1]
	s := util.ByteSliceToString(bs)
	f := util.OpenFile(that.dir + that.fileName)
	util.WriteStringToFile(f, s)
	util.CloseFile(f)
	rb.Reset()
}
