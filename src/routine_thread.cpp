#include <thread>
#include <iostream>
#include <string.h>
#include "routine_thread.hpp"
#include "scheduler.hpp"
#include <signal.h>
#include "monitor.hpp"

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

#ifdef __unix__
    pthread_create(&id, NULL, (void *(*)(void *))RoutineThread::threadRunFunc, this);

    int res = pthread_detach(id);
    if (res != 0)
    {
        std::cout << "phread_datch failed" << std::endl;
        return;
    }
    signal(SIGUSR2, monitor::blockingHandle);
    std::cout << "register thread id:" << id << std::endl;
#endif

#ifdef __WIN32
    std::thread t(RoutineThread::threadRunFunc, this);
    t.detach();
#endif

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
        rt->prevResumeTime = -1;
        if (so->status == ROUTINE_STATUS_FINISH)
        {
            so->status = ROUTINE_STATUS_READY;
            rt->sc->routinePool->giveback(so);
            rt->running = nullptr;
        }
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
    if (!this->getIsAccept())
    {
        return false;
    }
    routines.push(so);
    cond.notify_one();
    return true;
}

void RoutineThread::getFromWaitQueue()
{
    std::vector<Soroutine *> &ans = sc->pollRoutines(123);
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
        timeout = true;
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