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
    Soroutine *so = rb.getOne();
    return so;
}

bool Executor::addRoutine(Soroutine *routine)
{
    if (!routine)
    {
        return false;
    }
    std::unique_lock<std::mutex> lock(mu);
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
    std::unique_lock<std::mutex> lock(mu);
    while (activeRoutines.getSize() == 0)
    {
        cond.wait(lock);
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