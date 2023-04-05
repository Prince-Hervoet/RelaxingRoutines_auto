package core

import (
	"light_logger/src/util"
	"strconv"
	"sync"
	"time"
)

// 日志器
type LightLogger struct {
	current              *Buffer
	reserve              *Buffer
	maxBufferCount       int32
	bufferCount          int32
	singleBufferCapacity int32
	tempChannel          chan (*Buffer)
	lastUpdatedAt        int64
	dir                  string
	currentFileName      string
	lock                 *sync.Mutex
	cond                 *sync.Cond
}

func NewLightLogger(maxBufferCount, singleBufferCapacity int32) *LightLogger {
	ll := &LightLogger{}
	ll.current = NewBuffer(singleBufferCapacity)
	ll.reserve = NewBuffer(singleBufferCapacity)
	ll.maxBufferCount = maxBufferCount
	ll.bufferCount = 2
	ll.tempChannel = make(chan *Buffer, maxBufferCount)
	ll.dir = ""
	ll.lock = &sync.Mutex{}
	ll.singleBufferCapacity = singleBufferCapacity
	ll.cond = sync.NewCond(ll.lock)
	return ll
}

func (that *LightLogger) SetDir(dir string) {
	that.dir = dir
}

func (that *LightLogger) setReserve(newBuffer *Buffer) bool {
	that.lock.Lock()
	defer that.lock.Unlock()
	if that.reserve == nil {
		that.reserve = newBuffer
		that.cond.Broadcast()
		return true
	}
	that.bufferCount--
	return false
}

func (that *LightLogger) Start() {
	if that.dir != "" {
		that.lastUpdatedAt = time.Now().UnixMilli()
		that.currentFileName = strconv.FormatInt(that.lastUpdatedAt, 10) + ".txt"
		go func() {
			for buffer := range that.tempChannel {
				buffer.OutputToFile(that.dir + that.currentFileName)
				that.setReserve(buffer)
			}
		}()

		go func() {
			for {
				now := time.Now().UnixMilli()
				if now-2000 < that.lastUpdatedAt {
					continue
				}
				that.scan()
				that.lastUpdatedAt = time.Now().UnixMilli()
			}
		}()
	}

}

func (that *LightLogger) scan() {
	if that.lock.TryLock() {
		defer that.lock.Unlock()
		that.exchangeBuffer()
	}
}

func (that *LightLogger) exchangeBuffer() bool {
	if that.current.size == 0 {
		return false
	}
	if that.reserve == nil {
		if that.bufferCount < that.maxBufferCount {
			that.reserve = NewBuffer(that.singleBufferCapacity)
			that.bufferCount++
		} else {
			return false
		}
	}
	temp := that.current
	that.current = that.reserve
	that.tempChannel <- temp
	that.reserve = nil
	return true
}

func (that *LightLogger) Info(content string) {
	that.lock.Lock()
	defer that.lock.Unlock()
	data := util.GetNowTimeSimpleString() + " " + "[INFO] " + content + "\n"
	for that.current.Write(data) == -1 {
		if !that.exchangeBuffer() {
			that.cond.Wait()
		}
	}
}
