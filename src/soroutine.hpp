#pragma once
#include <ucontext.h>
#include <atomic>
#include "util.hpp"

static std::atomic<uint64_t> increment{0};

class Executor;
class Scheduler;
class RoutineThread;

class Soroutine
{
    friend class Scheduler;
    friend class RoutineThread;

private:
    uint64_t sid;        // id
    TaskFunc task;       // task pointer
    void *args;          // the args for the task
    char *runtimeStack;  // stackful
    int currentSize = 0; // current size of this stack
    int totalSize = 0;
    ucontext_t context;
    int status = ROUTINE_STATUS_INIT;

public:
    Soroutine(TaskFunc task, void *args);
    void setStackSize(int size);
    void toReady(int size);
    static void routineRunFunc(void *args);

    Soroutine()
    {
        this->sid = increment + 1;
    }

    ~Soroutine()
    {
        if (runtimeStack)
        {
            delete[] runtimeStack;
        }
    }

    void setTaskAndArgs(TaskFunc task, void *args)
    {
        this->task = task;
        this->args = args;
    }

    uint64_t getSid()
    {
        return sid;
    }
};
