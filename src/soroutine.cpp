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
            running->task(running->args);
        }
        catch (std::exception &e)
        {
            std::cout << running->sid << ": " << e.what() << std::endl;
        }
        running->status = ROUTINE_STATUS_DEAD;
    }
}

void Soroutine::setStackSize(int size)
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

/**
 * Allocate stack space and change the state to ready
 */
void Soroutine::toReady(int size)
{
    if (this->status != ROUTINE_STATUS_INIT)
    {
        return;
    }
    this->runtimeStack = new char[size];
    this->totalSize = size;
    this->currentSize = 0;
    this->status = ROUTINE_STATUS_READY;
}

Soroutine::Soroutine(TaskFunc task, void *args)
{
    this->sid = increment + 1;
    this->task = task;
    this->args = args;
    increment++;
}
