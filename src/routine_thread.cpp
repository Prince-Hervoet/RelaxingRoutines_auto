#include <thread>
#include <iostream>
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
        rt->resumeAccept();

        // rt->getFromWaitQueue();
    }
}

void RoutineThread::resumeAccept()
{
    if (isAccept)
    {
        return;
    }
    mu.lock();
    isAccept = true;
    prevResumeTime = -1;
    mu.unlock();
}

bool RoutineThread::addRoutine(Soroutine *so)
{
    std::unique_lock<std::mutex> lock(mu);
    if (!isAccept)
    {
        return false;
    }
    routines.push(so);
    cond.notify_one();
    return true;
}

void RoutineThread::getFromWaitQueue()
{

    std::vector<Soroutine *> &ans = sc->pollRoutines(ONCE_GET_WAIT_COUNT);
    std::unique_lock<std::mutex> lock(mu);
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

        // getFromWaitQueue();
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
    switch (so->status)
    {
    case ROUTINE_STATUS_INIT:
        sc->givebackRoutine(so);
        return;
    case ROUTINE_STATUS_PENDING:
    case ROUTINE_STATUS_READY:
        break;
    default:
        so->status = ROUTINE_STATUS_INIT;
        sc->givebackRoutine(so);
        return;
    }
    so->status = ROUTINE_STATUS_RUNNING;
    this->running = so;
    so->setContextLink(host);
    so->setContextMake(so->context, (void *)this);
    swapcontext(&host, &(so->context));
}

bool RoutineThread::solveTimeout()
{
    bool timeout = false;
    uint64_t now = getNowTimestamp();
    std::unique_lock<std::mutex> lock(mu);
    if (prevResumeTime != -1 && prevResumeTime + MAX_TIMEOUT_MS <= now)
    {
        isAccept = false;
        timeout = true;
        std::vector<Soroutine *> temp;
        for (int i = 0; i < routines.size(); i++)
        {
            temp.push_back(routines.front());
            routines.pop();
        }
        sc->pushRoutines(temp);
    }
    return timeout;
}