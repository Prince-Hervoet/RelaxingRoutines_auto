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
    uint64_t sid;        // id
    TaskFunc task;       // task pointer
    void *args;          // the args for the task
    char *runtimeStack;  // stackful
    int currentSize = 0; // current size of this stack
    int totalSize = 0;
    ucontext_t context;
    int status = ROUTINE_STATUS_INIT;

public:
    Soroutine(int sid);
    Soroutine(int sid, int size);
    Soroutine(int sid, int size, TaskFunc task, void *args);
    void setStackSize(int size);
    void initContext();
    void setContextLink(ucontext_t &context);
    void setContextMake(ucontext_t &context, void *args);
    static void routineRunFunc(void *args);

    Soroutine()
    {
        runtimeStack = new char[512];
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
