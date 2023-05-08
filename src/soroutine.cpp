#include <string.h>
#include <iostream>
#include "routine_thread.hpp"
#include "scheduler.hpp"
#include "soroutine.hpp"

void Soroutine::routineRunFunc(void *args)
{
    RoutineThread *rt = (RoutineThread *)args;
    Soroutine *running = rt->getRunning();
    if (running)
    {
        try
        {
            running->task(args);
        }
        catch (std::exception &e)
        {
        }
        running->status = ROUTINE_STATUS_INIT;
        rt->sc->routinePool->giveback(running);
        std::cout << "check 11123" << std::endl;
    }
}

void Soroutine::initContext()
{
    memset(runtimeStack, 0, totalSize);
    getcontext(&context);
    context.uc_stack.ss_sp = runtimeStack;
    context.uc_stack.ss_size = totalSize;
    context.uc_stack.ss_flags = 0;
    status = ROUTINE_STATUS_READY;
}

void Soroutine::setContextLink(ucontext_t &context)
{
    context.uc_link = &context;
}

void Soroutine::setContextMake(ucontext_t &context, void *args)
{
    makecontext(&context, (void (*)())Soroutine::routineRunFunc, 1, args);
}

void Soroutine::setStackSize(int size)
{
    {
        if (size <= 0 || size == totalSize)
        {
            return;
        }
        if (runtimeStack)
        {
            delete[] runtimeStack;
            runtimeStack = new char[size];
            currentSize = 0;
            totalSize = size;
        }
    }
}

Soroutine::Soroutine(int sid)
{
    this->sid = sid;
}

Soroutine::Soroutine(int sid, int size)
{
    size = size <= 0 ? 512 : size;
    this->sid = sid;
    this->runtimeStack = new char[size];
    totalSize = size;
}

Soroutine::Soroutine(int sid, int size, TaskFunc task, void *args)
{
    this->sid = sid;
    size = size <= 0 ? 512 : size;
    this->sid = sid;
    this->runtimeStack = new char[size];
    totalSize = size;
    this->task = task;
    this->args = args;
}
