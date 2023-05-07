#include "scheduler.hpp"

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
        so->setStackSize(512);
    }
    so->setTaskAndArgs(task, args);
    so->initContext(Soroutine::routineRunFunc);
    so->status = ROUTINE_STATUS_READY;
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
        rts[index]->addRoutine(so);
        return;
    }
    for (int i = 0; i < count; i++)
    {
        if (rts[i]->getIsAccept())
        {
            rts[i]->addRoutine(so);
            return;
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
    RoutineThread *n = new RoutineThread();
    n->start();
    n->addRoutine(so);
    rts.push_back(n);
}

std::vector<Soroutine *> &Scheduler::pollRoutines(int count)
{
    std::vector<Soroutine *> *ans = new std::vector<Soroutine *>();
    std::unique_lock<std::mutex> lock(mu);
    while (!waitQueue.empty() && count > 0)
    {
        Soroutine *so = waitQueue.front();
        waitQueue.pop();
        ans->push_back(so);
        count -= 1;
    }
    return *ans;
}