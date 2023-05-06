#include "scheduler.hpp"
#include "executor.hpp"
#include "contextPool.hpp"
#include <string.h>
#include <thread>

void Scheduler::addTask(TaskFunc task, void *args)
{
    contextPool *cp = new contextPool();
    Soroutine *packing = cp->getContext();
    if (packing)
    {
        packing->setTask(task, args);
        packing->setStatus(ROUTINE_READY);
        ucontext_t context = packing->getContext();
        // todo: set context
        if (sem_trywait(&sem) == -1)
        {
            // if go here,the sem is zero.
            globalTaskQueue.add(*packing);
        }
    }
}

void Scheduler::setRoutineInfo(Soroutine *so, TaskFunc task, void *args)
{
    if (!so)
    {
        return;
    }
    so->setTask(task, args);
}

Scheduler::Scheduler()
{
    systemCoreSize = std::thread::hardware_concurrency();
    int temp = systemCoreSize * 2;
    for (int i = 0; i < temp; i++)
    {
        Executor *executor = new Executor();
        executors.push_back(executor);
    }
    sem_init(&sem, 0, temp);
}