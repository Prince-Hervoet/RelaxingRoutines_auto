#include "executor.hpp"

void Executor::taskRunningFunc(Executor *executor)
{
    if (executor->running)
    {
        (executor->running->getTask())(executor->running->getArgs());
    }
}

bool Executor::addRoutine(Soroutine *routine)
{
    if (!routine)
    {
        return false;
    }
    activeRoutines.add(*routine);
    cond.notify_one();
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

Soroutine *Executor::getActiveRoutine()
{
    if (activeRoutines.getSize() == 0)
    {
        return nullptr;
    }
    Soroutine *so = activeRoutines.getFirst();
    return so;
}

void Executor::resumeRoutine()
{
    for (;;)
    {
        Soroutine *so = getActiveRoutine();
        if (so)
        {
            prevResumeTimestamp = getNowTimestamp();
            // 运行so
        }
        if (status == EXECUTOR_STOP)
        {
            break;
        }
    }
}