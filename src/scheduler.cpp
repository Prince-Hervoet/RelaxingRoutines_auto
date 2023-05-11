#include <thread>
#include "scheduler.hpp"
#include <iostream>

Scheduler &Scheduler::getInstance()
{
    static Scheduler s;
    return s;
}

/**
 * Gets a usable coroutine structure
 */
Soroutine *Scheduler::createRoutine(TaskFunc task, void *args)
{
    Soroutine *so = this->routinePool->getRoutine();
    if (!so)
    {
        so = new Soroutine();
    }
    so->toReady(READY_STACK_SIZE);
    so->setTaskAndArgs(task, args);
    return so;
}

void Scheduler::givebackRoutine(Soroutine *so)
{
    if (!so)
    {
        return;
    }
    this->routinePool->giveback(so);
}

/**
 * Pick a thread and drop the task into it
 */
int Scheduler::addTask(TaskFunc task, void *args)
{
    Soroutine *so = this->createRoutine(task, args);
    uint64_t sid = so->getSid();
    int count = this->rts.size();
    int index = sid % count;
    // choose a thread
    if (rts[index]->getIsAccept())
    {
        if (rts[index]->addRoutine(so))
        {
            return 1;
        }
    }
    for (int i = 0; i < count; i++)
    {
        if (rts[i]->getIsAccept())
        {
            if (rts[i]->addRoutine(so))
            {
                return 1;
            }
        }
    }
    if (this->createRoutineThread(so))
    {
        return 1;
    }
    return 0;
}

/**
 * create a thread
 */
int Scheduler::createRoutineThread(Soroutine *so)
{
    if (!so || rts.size() == MAX_THREAD_COUNT)
    {
        return 0;
    }
    std::unique_lock<std::mutex> lock(mu);
    if (rts.size() == MAX_THREAD_COUNT)
    {
        return 0;
    }
    RoutineThread *n = new RoutineThread(this);
    n->start();
    n->addRoutine(so);
    rts.push_back(n);
    return 1;
}

std::vector<Soroutine *> &Scheduler::pollRoutines(int count)
{
    std::vector<Soroutine *> *ans = new std::vector<Soroutine *>();
    mu.lock();
    while (!waitQueue.empty() && count > 0)
    {
        Soroutine *so = waitQueue.front();
        waitQueue.pop();
        ans->push_back(so);
        count -= 1;
    }
    mu.unlock();
    return *ans;
}

void Scheduler::pushRoutines(std::vector<Soroutine *> &routines)
{
    std::unique_lock<std::mutex> lock(mu);
    for (int i = 0; i < routines.size(); i++)
    {
        waitQueue.push(routines[i]);
    }
}

/**
 * init something
 */
Scheduler::Scheduler()
{
    this->mo = new monitor();
    this->routinePool = new BufferPool();
    unsigned int core = std::thread::hardware_concurrency();
    for (int i = 0; i < 1; i++)
    {
        RoutineThread *rt = new RoutineThread(this);
        this->rts.push_back(rt);
    }
    for (int i = 0; i < 1; i++)
    {
        rts[i]->start();
    }
    mo->start(rts);
    // std::thread t(Scheduler::monitor, this);
    // t.detach();
}