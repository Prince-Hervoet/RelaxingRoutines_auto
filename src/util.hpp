#pragma once
#include <cstdint>

#define ROUTINE_INIT 10
#define ROUTINE_READY 11
#define ROUTINE_RUNNING 12
#define ROUTINE_PENDING 13

#define STACK_SIZE 131072

typedef void (*TaskFunc)(void *);

uint64_t getNowTimestamp();