#pragma once
#include <cstdint>

typedef void (*TaskFunc)(void *args);
typedef void (*ExecutorFunc)(void *args);

#define TASK_MAX_TIMEOUT 50

/**
 * status code for the routine
 */
#define ROUTINE_STATUS_INIT 10
#define ROUTINE_STATUS_READY 20
#define ROUTINE_STATUS_RUNNING 30
#define ROUTINE_STATUS_PENDING 40
#define ROUTINE_STATUS_DEAD 50

/**
 * status code for the
 */
#define EXECUTOR_STATUS_RUNNING 100
#define EXECUTOR_STATUS_PENDING 200
#define EXECUTOR_STATUS_STOP 300

/**
 * status code for the RoutineThread
 */
#define ROUTINE_THREAD_LEADER 22
#define ROUTINE_THREAD_HELPER 33
#define ROUTINE_THREAD_PENDING 44

/**
 * some information
 */
#define MAX_ROUTINE_THREAD 1024
#define CORE_ROUTINE_THREAD 16
#define MAX_TIMEOUT_MS 20

/**
 * get routines from the waitQueue
 */
#define ONCE_GET_WAIT_COUNT 128

uint64_t getNowTimestamp();