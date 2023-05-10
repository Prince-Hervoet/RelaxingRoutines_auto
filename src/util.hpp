#pragma once
#include <cstdint>

typedef void (*TaskFunc)(void *args);
typedef void (*ExecutorFunc)(void *args);

#define TASK_MAX_TIMEOUT 50

/**
 * status code for the routine
 */
#define ROUTINE_STATUS_INIT 10    // It has just been created and no stack space has been allocated yet
#define ROUTINE_STATUS_READY 20   // Allocate stack space to it and transition to the ready status
#define ROUTINE_STATUS_RUNNING 30 // Set up your environment and start running, changing to running status
#define ROUTINE_STATUS_PENDING 40 // The runtime is suspended, transitioning to pending status
#define ROUTINE_STATUS_FINISH 50  //

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
#define MAX_TIMEOUT_MS 50

/**
 * get routines from the waitQueue
 */
#define ONCE_GET_WAIT_COUNT 128

#define READY_STACK_SIZE 16384

uint64_t getNowTimestamp();