#ifndef __SOROUTINE_H__
#define __SOROUTINE_H__

#include <ucontext.h>
#include "util.hpp"

class Executor;
class Soroutine
{
private:
    char *myStack;
    int size = 0;
    int alreadySize = 0;
    TaskFunc task;
    void *args;
    ucontext_t context;
    int status = ROUTINE_INIT;

public:
    Soroutine(TaskFunc task, void *args);
    Soroutine(TaskFunc task, void *args, int stackSize);
    void setStack(int newSize);

    Soroutine()
    {
    }

    ~Soroutine()
    {
        if (myStack)
        {
            delete[] myStack;
        }
    }

    void setStatus(int status)
    {
        this->status = status;
    }

    void setTask(TaskFunc task, void *args)
    {
        this->task = task;
        this->args = args;
    }

    int getSize()
    {
        return size;
    }

    int getStackSize()
    {
        return alreadySize;
    }

    TaskFunc getTask()
    {
        return task;
    }

    void *getArgs()
    {
        return args;
    }

    ucontext_t &getContext()
    {
        return context;
    }
};

#endif