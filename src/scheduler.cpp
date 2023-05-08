#include <thread>
#include "scheduler.hpp"
#include <iostream>

Scheduler &Scheduler::getInstance()
{
    static Scheduler s;
    return s;
}

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

void Scheduler::addTask(TaskFunc task, void *args)
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
            return;
        }
    }
    for (int i = 0; i < count; i++)
    {
        if (rts[i]->getIsAccept())
        {
            if (rts[i]->addRoutine(so))
            {
                return;
            }
        }
    }
    this->createRoutineThread(so);
}

void Scheduler::createRoutineThread(Soroutine *so)
{
    if (!so || rts.size() == MAX_ROUTINE_THREAD)
    {
        return;
    }
    RoutineThread *n = new RoutineThread(this);
    n->start();
    n->addRoutine(so);
    rts.push_back(n);
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

void Scheduler::monitor(Scheduler *sc)
{
    auto rts = sc->rts;
    for (;;)
    {
        for (int i = 0; i < rts.size(); i++)
        {
            rts[i]->solveTimeout();
        }
    }
}

Scheduler::Scheduler()
{
    this->routinePool = new BufferPool();
    unsigned int core = std::thread::hardware_concurrency();
    for (int i = 0; i < core; i++)
    {
        RoutineThread *rt = new RoutineThread(this);
        this->rts.push_back(rt);
    }
    for (int i = 0; i < core; i++)
    {
        rts[i]->start();
    }
    // std::thread t(Scheduler::monitor, this);
    // t.detach();
}