#pragma once
#include "util.hpp"
#include <ucontext.h>

class Executor;
class Scheduler;
class RoutineThread;

class Soroutine
{
    friend class Scheduler;
    friend class RoutineThread;

private:
    uint64_t sid;  // id
    TaskFunc task; // task pointer
    void *args;
    char *runtimeStack;
    int currentSize = 0;
    int totalSize = 0;
    Executor *et;
    ucontext_t context;
    int status = ROUTINE_STATUS_INIT;

public:
    Soroutine(int sid);
    Soroutine(int sid, int size);
    Soroutine(int sid, int size, TaskFunc task, void *args);
    void setStackSize(int size);
    void initContext(ExecutorFunc func);
    static void routineRunFunc(void *args);

    Soroutine()
    {
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
