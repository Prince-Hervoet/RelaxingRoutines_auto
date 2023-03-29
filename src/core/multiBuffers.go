package core

import "time"

// 默认缓冲区容量
const DEFAULT_BUFFER_CAPACITY = 32768

// 默认缓冲区数量
const DEFAULT_BUFFER_COUNT = 5

// 默认时间间隔(ms)
const DEFAULT_SCAN_INTERVAL = 200

type MultiBuffers struct {
	// 当前使用的缓冲区
	currentBuffer *RingLightBuffer
	// 空闲缓冲区
	freeBuffers []*RingLightBuffer
	// 正在持久化的缓冲区
	busyBuffers []*RingLightBuffer
	// 缓冲区数量上限
	bufferCountLimit int32
	// 当前缓冲区数量
	bufferCount int32
	// 单个缓冲区大小(byte)
	bufferSize int32
	// 空闲时扫描间隔(ms)
	interval int64
	// 最近一次更新时间戳
	prevTimestamp int64
}

func newMultiBuffers(bufferSize, bufferCountLimit int32) *MultiBuffers {
	if bufferSize <= 0 {
		bufferSize = DEFAULT_BUFFER_CAPACITY
	}
	if bufferCountLimit <= 0 {
		bufferCountLimit = DEFAULT_BUFFER_COUNT
	}
	// 初始化两个缓冲区
	lb1 := newLightBuffer(bufferSize)
	lb2 := newLightBuffer(bufferSize)
	now := time.Now().UnixMilli()
	mb := &MultiBuffers{}
	mb.bufferCount = 2
	mb.bufferCountLimit = bufferCountLimit
	mb.bufferSize = bufferSize
	mb.freeBuffers = make([]*RingLightBuffer, 1, bufferCountLimit)
	mb.busyBuffers = make([]*RingLightBuffer, 0, bufferCountLimit)
	mb.prevTimestamp = now
	mb.interval = DEFAULT_SCAN_INTERVAL
	mb.currentBuffer = lb1
	mb.freeBuffers[0] = lb2
	return mb
}

func (that *MultiBuffers) write(bytes []byte) {
	that.currentBuffer.hold.Lock()
	defer that.currentBuffer.hold.Unlock()
	if that.currentBuffer.write(bytes) == 0 {

	}
}
