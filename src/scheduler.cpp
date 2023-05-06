#include "scheduler.hpp"
#include "executor.hpp"
#include <string.h>
#include <thread>

void Scheduler::addTask(TaskFunc task, void *args)
{
    Soroutine *packing = cp->getContext();
    if (packing)
    {
        packing->setTask(task, args);
        packing->setStatus(ROUTINE_READY);
        ucontext_t context = packing->getContext();
        // todo: set context

        Executor *executor = this->freeExecutors.poll();
        if (executor)
        {
            executor->addToLocalQueue(packing);
        }
        else
        {
            this->globalTaskQueue.add(*packing);
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
    for (int i = 0; i < systemCoreSize; i++)
    {
        Executor *executor = new Executor();
        executors.push_back(executor);
    }
    cp = new contextPool();
    sem_init(&sem, 0, systemCoreSize);
}