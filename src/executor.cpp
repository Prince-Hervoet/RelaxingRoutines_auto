#include "executor.hpp"

void Executor::taskRunningFunc(Executor *executor)
{
    executor->status = EXECUTOR_RUNNING;
    if (executor->running)
    {
        (executor->running->getTask())(executor->running->getArgs());
    }
    executor->rb.giveBack(executor->running);
    executor->running = nullptr;
    executor->status = EXECUTOR_WAIT;
}

Soroutine *Executor::getBuffer()
{
    mu.lock();
    Soroutine *so = rb.getOne();
    mu.unlock();
    return so;
}

bool Executor::addRoutine(Soroutine *routine)
{
    if (!routine)
    {
        return false;
    }
    mu.lock();
    activeRoutines.add(*routine);
    mu.unlock();
    return true;
}

bool Executor::isTimeout()
{
    uint64_t now = getNowTimestamp();

    if (status == EXECUTOR_RUNNING && now - prevResumeTimestamp >= timeout)
    {
        return true;
    }
    return false;
}

void Executor::resumeRoutine()
{
    for (;;)
    {
        while (activeRoutines.getSize() > 0)
        {
            Soroutine *so = activeRoutines.getFirst();
        }
        if (status == EXECUTOR_STOP)
        {
            break;
        }
    }
}