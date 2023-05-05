#pragma once
#include <cstdint>
#include <chrono>

#define ROUTINE_INIT 10
#define ROUTINE_READY 11
#define ROUTINE_RUNNING 12
#define ROUTINE_PENDING 13

const int EXECUTOR_WAIT = 0;
const int EXECUTOR_RUNNING = 1;
const int EXECUTOR_PENDING = 2;
const int EXECUTOR_STOP = -1;

#define STACK_SIZE 131072

typedef void (*TaskFunc)(void *);

uint64_t getNowTimestamp();