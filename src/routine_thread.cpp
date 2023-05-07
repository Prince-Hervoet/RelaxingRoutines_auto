#include <thread>
#include "routine_thread.hpp"
#include "scheduler.hpp"

void RoutineThread::start()
{
    if (isStart)
    {
        return;
    }
    isStart = true;
    std::thread t(RoutineThread::threadRunFunc, this);
    t.detach();
    isAccept = true;
}

void RoutineThread::threadRunFunc(void *args)
{
    RoutineThread *rt = (RoutineThread *)args;
    for (;;)
    {
        Soroutine *so = rt->pollRoutine();
        rt->prevResumeTime = getNowTimestamp();
        rt->resumeRoutine(so);
        if (!rt->isAccept)
        {
            rt->isAccept = true;
        }
    }
}

void RoutineThread::addRoutine(Soroutine *so)
{
    std::unique_lock<std::mutex> lock(mu);
    routines.push(so);
    cond.notify_one();
}

void RoutineThread::getFromWaitQueue()
{
    std::vector<Soroutine *> &ans = sc->pollRoutines(ONCE_GET_WAIT_COUNT);
    for (int i = 0; i < ans.size(); i++)
    {
        this->routines.push(ans[i]);
    }
    delete &ans;
}

Soroutine *RoutineThread::pollRoutine()
{
    std::unique_lock<std::mutex> lock(mu);
    while (routines.size() == 0)
    {
        getFromWaitQueue();
        if (routines.size() == 0)
        {
            cond.wait(lock);
        }
        else
        {
            break;
        }
    }
    Soroutine *so = routines.front();
    routines.pop();
    return so;
}

void RoutineThread::resumeRoutine(Soroutine *so)
{
    if (!so)
    {
        return;
    }
    this->running = so;
    swapcontext(&host, &(so->context));
}