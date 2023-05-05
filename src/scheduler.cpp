#include "scheduler.hpp"
#include "executor.hpp"

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
                Soroutine *old = executor->getBuffer();
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