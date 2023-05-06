#include "scheduler.hpp"
#include "executor.hpp"
#include <string.h>
#include <thread>

void Scheduler::addTask(TaskFunc task)
{
    for (int i = 0; i < executors.size(); i++)
    {
        Executor *executor = executors[i];
        bool isOk = false;
        if (executor->getSize() < ROUTINE_QUEUE_SIZE)
        {
            mu.lock();
            if (executor->getSize() < ROUTINE_QUEUE_SIZE)
            {
                isOk = true;
                Soroutine *old = nullptr;
                if (old)
                {
                    executor->addRoutine(old);
                }
                else
                {
                    Soroutine *so = this->createRoutine();
                    executor->addRoutine(so);
                }
            }
            mu.unlock();
        }
        if (isOk)
        {
            break;
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
}