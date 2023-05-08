#include <thread>
#include <iostream>
#include <string.h>
#include "routine_thread.hpp"
#include "scheduler.hpp"

/**
 * start a thread
 */
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

/**
 *  Functions to execute when a thread is started
 */
void RoutineThread::threadRunFunc(void *args)
{
    RoutineThread *rt = (RoutineThread *)args;
    for (;;)
    {
        Soroutine *so = rt->pollRoutine();
        rt->prevResumeTime = getNowTimestamp();
        rt->resumeRoutine(so);
        so->status = ROUTINE_STATUS_READY;
        rt->sc->routinePool->giveback(so);
        rt->running = nullptr;
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
    for (int i = 0; i < ans.size(); i++)
    {
        this->routines.push(ans[i]);
    }
    delete &ans;
}

/**
 * get a routine and wait for it
 */
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

/**
 * start a routine
 */
void RoutineThread::resumeRoutine(Soroutine *so)
{
    if (!so)
    {
        return;
    }
    switch (so->status)
    {
    case ROUTINE_STATUS_READY:
        memset(so->runtimeStack, 0, so->totalSize);
        getcontext(&so->context);
        so->context.uc_link = &host;
        so->context.uc_stack.ss_sp = so->runtimeStack;
        so->context.uc_stack.ss_size = so->totalSize;
        makecontext(&so->context, (void (*)())Soroutine::routineRunFunc, 1, (void *)this);
        break;
    case ROUTINE_STATUS_PENDING:
        break;
    default:
        so->status = ROUTINE_STATUS_INIT;
        sc->givebackRoutine(so);
        return;
    }
    this->running = so;
    so->status = ROUTINE_STATUS_RUNNING;
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

RoutineThread::RoutineThread()
{
}

RoutineThread::RoutineThread(Scheduler *sc)
{
    this->sc = sc;
}