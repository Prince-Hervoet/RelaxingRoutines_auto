#include "executor.hpp"

bool Executor::addToLocalQueue(Soroutine *routine)
{
    if (!routine)
    {
        return false;
    }
    localRoutines.add(*routine);
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
    if (localRoutines.getSize() == 0)
    {
        return nullptr;
    }
    Soroutine *so = localRoutines.poll();
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