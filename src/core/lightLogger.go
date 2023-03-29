package core

import "time"

type LightLogger struct {
	mbs    *MultiBuffers
	dir    string
	active bool
}

func NewLightLogger() {

}

func (logger *LightLogger) Start() {
	// 后台
	go func() {
		for {
			now := time.Now().UnixMilli()

		}
	}()
}

func (logger *LightLogger) Info(msg string) {

}

func (logger *LightLogger) Warn(msg string) {

}

func (logger *LightLogger) Error(msg string) {

}
